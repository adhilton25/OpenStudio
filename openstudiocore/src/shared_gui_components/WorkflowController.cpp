/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/


#include "WorkflowController.hpp"
#include "WorkflowView.hpp"
#include "MeasureDragData.hpp"
#include "MeasureManager.hpp"
#include "EditController.hpp"
#include "BaseApp.hpp"
#include "LocalLibraryController.hpp"
#include "WorkflowTools.hpp"

#include "../energyplus/ForwardTranslator.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/RubyException.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/filetypes/WorkflowStep_Impl.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include <QByteArray>
#include <QDialog>
#include <QDropEvent>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QRadioButton>


namespace openstudio{


namespace measuretab {

WorkflowController::WorkflowController(BaseApp *t_app)
  : OSListController()
{
  QSharedPointer<WorkflowSectionItem> workflowSectionItem;

  workflowSectionItem = QSharedPointer<WorkflowSectionItem>(new WorkflowSectionItem(MeasureType::ModelMeasure, "OpenStudio Measures", t_app));
  addItem(workflowSectionItem);

  workflowSectionItem = QSharedPointer<WorkflowSectionItem>(new WorkflowSectionItem(MeasureType::EnergyPlusMeasure, "EnergyPlus Measures", t_app));
  addItem(workflowSectionItem);

  workflowSectionItem = QSharedPointer<WorkflowSectionItem>(new WorkflowSectionItem(MeasureType::ReportingMeasure, "Reporting Measures", t_app));
  addItem(workflowSectionItem);
}

void WorkflowController::addItem(QSharedPointer<OSListItem> item)
{
  if( QSharedPointer<WorkflowSectionItem> workflowSectionItem = item.dynamicCast<WorkflowSectionItem>() )
  {
    m_workflowSectionItems.push_back(workflowSectionItem);
    workflowSectionItem->setController(this);
    workflowSectionItem->workflowStepController()->setSelectionController(selectionController());
  }
}

QSharedPointer<OSListItem> WorkflowController::itemAt(int i)
{
  if( i >= 0 && i < count() )
  {
    return m_workflowSectionItems[i];
  }

  return QSharedPointer<OSListItem>();
}

int WorkflowController::count()
{
  return m_workflowSectionItems.size();
}

WorkflowSectionItem::WorkflowSectionItem(MeasureType measureType, const QString & label, BaseApp *t_baseApp)
  : OSListItem(),
    m_label(label),
    m_measureType(measureType)
{
  m_workflowStepController = QSharedPointer<WorkflowStepController>(new MeasureStepController(measureType, t_baseApp));
}

QString WorkflowSectionItem::label() const
{
  return m_label;
}

QSharedPointer<WorkflowStepController> WorkflowSectionItem::workflowStepController() const
{
  return m_workflowStepController;
}

WorkflowSectionItemDelegate::WorkflowSectionItemDelegate()
{
}

QWidget * WorkflowSectionItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if( QSharedPointer<WorkflowSectionItem> workflowSectionItem = dataSource.objectCast<WorkflowSectionItem>() )
  {
    QSharedPointer<WorkflowStepController> workflowStepController = workflowSectionItem->workflowStepController();

    if (QSharedPointer<MeasureStepController> measureStepController = qSharedPointerCast<MeasureStepController>(workflowStepController)){
      
      QSharedPointer<MeasureStepItemDelegate> measureStepItemDelegate = QSharedPointer<MeasureStepItemDelegate>(new MeasureStepItemDelegate());

      MeasureType measureType = measureStepController->measureType();

      QString acceptedMimeType = MeasureDragData::mimeType(measureType);

      auto workflowSectionView = new WorkflowSectionView(measureType);

      workflowSectionView->content->newMeasureDropZone->setAcceptedMimeType(acceptedMimeType);

      connect(workflowSectionView->content->newMeasureDropZone, &NewMeasureDropZone::dataDropped, measureStepController.data(), &MeasureStepController::addItemForDroppedMeasure);

      workflowSectionView->content->workflowStepsView->setListController(measureStepController);
      workflowSectionView->content->workflowStepsView->setDelegate(measureStepItemDelegate);

      workflowSectionView->header->label->setText(workflowSectionItem->label());

      return workflowSectionView;
    }
  }

  return new QWidget();
}

WorkflowStepController::WorkflowStepController(openstudio::BaseApp *t_app)
  : OSListController()
{}

MeasureStepController::MeasureStepController(MeasureType measureType, openstudio::BaseApp *t_app)
  : WorkflowStepController(t_app),
    m_app(t_app),
    m_measureType(measureType)
{
}

MeasureType MeasureStepController::measureType() const
{
  return m_measureType;
}

std::vector<MeasureStep> MeasureStepController::measureSteps() const
{
  WorkflowJSON workflowJSON = m_app->currentModel()->workflowJSON();
  std::vector<MeasureStep> result = workflowJSON.getMeasureSteps(m_measureType);
  return result;
}

QSharedPointer<OSListItem> MeasureStepController::itemAt(int i)
{
  std::vector<MeasureStep> steps = this->measureSteps();

  if( i >= 0 && i < (int)steps.size() )
  {
    MeasureStep step = steps[i];

    QSharedPointer<MeasureStepItem> item = QSharedPointer<MeasureStepItem>(new MeasureStepItem(m_measureType, step, m_app));

    item->setController(this);

    return item;
  }

  return QSharedPointer<MeasureStepItem>();
}

int MeasureStepController::count()
{
  return measureSteps().size();
}

void MeasureStepController::removeItemForStep(MeasureStep step)
{
  std::vector<MeasureStep> oldMeasureSteps = measureSteps();

  bool didRemove = false;
  std::vector<MeasureStep> newMeasureSteps;
  newMeasureSteps.reserve(oldMeasureSteps.size());
  for (const auto& oldMeasureStep : oldMeasureSteps){
    if (oldMeasureStep == step){
      didRemove = true;
    } else{
      newMeasureSteps.push_back(oldMeasureStep);
    }
  }

  if (didRemove){
    m_app->currentModel()->workflowJSON().setMeasureSteps(m_measureType, newMeasureSteps);

    emit modelReset();
  }
}

void MeasureStepController::addItemForDroppedMeasure(QDropEvent *event)
{
  // accept the event to make the icon refresh
  event->accept();

  const QMimeData * mimeData = event->mimeData();

  QByteArray byteArray;

  byteArray = mimeData->data(MeasureDragData::mimeType(m_measureType));

  MeasureDragData measureDragData(byteArray);

  UUID id = measureDragData.id();

  boost::optional<BCLMeasure> projectMeasure;
  try {

    // Get the measure, will throw if error occurs
    projectMeasure = m_app->measureManager().insertReplaceMeasure(id);

  } catch (const std::exception& e) {

    QString errorMessage("Failed to add measure: \n\n");
    errorMessage += QString::fromStdString(e.what());
    QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
    return;
  }
  OS_ASSERT(projectMeasure);

  if (projectMeasure->measureType() != m_measureType){
    QString errorMessage("Failed to add measure at this workflow location.");
    QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
    return;
  }

  MeasureStep measureStep(toString(projectMeasure->directory().stem()));
  try{
    std::vector<measure::OSArgument> arguments = m_app->measureManager().getArguments(*projectMeasure);
  } catch ( const RubyException&e ) {
    QString errorMessage("Failed to compute arguments for measure: \n\n");
    errorMessage += QString::fromStdString(e.what());
    QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
    return;
  }

  // the new measure
  std::string name = m_app->measureManager().suggestMeasureName(*projectMeasure);
  measureStep.setName(name);
  //measureStep.setDisplayName(name); // DLM: TODO
  measureStep.setDescription(projectMeasure->description());
  measureStep.setModelerDescription(projectMeasure->modelerDescription());
      
  WorkflowJSON workflowJSON = m_app->currentModel()->workflowJSON();

  std::vector<MeasureStep> measureSteps = workflowJSON.getMeasureSteps(m_measureType);
  measureSteps.push_back(measureStep);
  bool test = workflowJSON.setMeasureSteps(m_measureType, measureSteps);
  OS_ASSERT(test);

  //workflowJSON.save();

  emit modelReset();
}

void MeasureStepController::moveUp(MeasureStep step)  
{
  std::vector<MeasureStep> oldMeasureSteps = measureSteps();
  
  bool found = false;
  size_t i;
  size_t n = oldMeasureSteps.size();
  for (i = 0; i < n; ++i){
    if (oldMeasureSteps[i] == step){
      found = true;
      break;
    }
  }

  if( found && i > 0 )
  {
    // swap i with i-1
    MeasureStep temp = oldMeasureSteps[i-1];
    oldMeasureSteps[i-1] = oldMeasureSteps[i];
    oldMeasureSteps[i] = temp;

    m_app->currentModel()->workflowJSON().setMeasureSteps(m_measureType, oldMeasureSteps);

    emit itemChanged(i-1);
    emit itemChanged(i);
  }
}


void MeasureStepController::moveDown(MeasureStep step)
{
  std::vector<MeasureStep> oldMeasureSteps = measureSteps();
  
  bool found = false;
  size_t i;
  size_t n = oldMeasureSteps.size();
  for (i = 0; i < n; ++i){
    if (oldMeasureSteps[i] == step){
      found = true;
      break;
    }
  }

  if( found && i < (n-1) )
  {
    // swap i with i+1
    MeasureStep temp = oldMeasureSteps[i+1];
    oldMeasureSteps[i+1] = oldMeasureSteps[i];
    oldMeasureSteps[i] = temp;

    m_app->currentModel()->workflowJSON().setMeasureSteps(m_measureType, oldMeasureSteps);

    emit itemChanged(i);
    emit itemChanged(i+1);
  }
}

MeasureStepItem::MeasureStepItem(MeasureType measureType, MeasureStep step, openstudio::BaseApp *t_app)
  : OSListItem(),
    m_app(t_app),
    m_measureType(measureType),
    m_step(step)
{
}

QString MeasureStepItem::name() const
{ 
  QString result;
  if (boost::optional<std::string> name = m_step.name()){
    return result = QString::fromStdString(*name);
  }
  return result;
}

QString MeasureStepItem::displayName() const
{
  // DLM: TODO, add display name
  QString result;
  if (boost::optional<std::string> name = m_step.name()){
    return result = QString::fromStdString(*name);
  }
  return result;
}

MeasureType MeasureStepItem::measureType() const
{
  OptionalBCLMeasure bclMeasure = this->bclMeasure();
  OS_ASSERT(bclMeasure);
  return bclMeasure->measureType();
}


MeasureStep MeasureStepItem::measureStep() const
{
  return m_step;
}

QString MeasureStepItem::description() const
{
  QString result;
  if (boost::optional<std::string> description = m_step.description()){
    return result = QString::fromStdString(*description);
  }
  return result;
}

QString MeasureStepItem::modelerDescription() const
{
  QString result;
  if (boost::optional<std::string> modelerDescription = m_step.modelerDescription()){
    return result = QString::fromStdString(*modelerDescription);
  }
  return result;
}

QString MeasureStepItem::scriptFileName() const
{
  QString result;
  OptionalBCLMeasure bclMeasure = this->bclMeasure();
  if (bclMeasure){
    boost::optional<path> primaryRubyScriptPath = bclMeasure->primaryRubyScriptPath();
    if (primaryRubyScriptPath){
      result = toQString(*primaryRubyScriptPath);
    }
  }
  return result;
}

OptionalBCLMeasure MeasureStepItem::bclMeasure() const
{
  return m_app->currentModel()->workflowJSON().getBCLMeasure(m_step);
}

std::vector<measure::OSArgument> MeasureStepItem::arguments() const
{
  std::vector<measure::OSArgument> result;

  // get arguments from the BCL Measure (computed using the current model)
  OptionalBCLMeasure bclMeasure = this->bclMeasure();
  if (bclMeasure){
    result = m_app->measureManager().getArguments(*bclMeasure);
  }

  // fill in with any arguments in this WorkflowJSON
  for (auto& argument : result){
     boost::optional<Variant> variant = m_step.getArgument(argument.name());
     if (variant){

       VariantType variantType = variant->variantType();
       if (variantType == VariantType::Boolean){
         argument.setValue(variant->valueAsBoolean());
       } else if (variantType == VariantType::Integer){
         argument.setValue(variant->valueAsInteger());
       } else if (variantType == VariantType::Double){
         argument.setValue(variant->valueAsDouble());
       } else if (variantType == VariantType::String){
         argument.setValue(variant->valueAsString());
       }
     }
  }

  return result;
}

bool MeasureStepItem::hasIncompleteArguments() const
{
  std::vector<measure::OSArgument> arguments;

  // get arguments from the BCL Measure (computed using the current model)
  OptionalBCLMeasure bclMeasure = this->bclMeasure();
  if (bclMeasure){
    arguments = m_app->measureManager().getArguments(*bclMeasure);
  }

  // find any required arguments without a value
  for (const auto& argument : arguments){
    if (argument.required() && !argument.hasDefaultValue()){
      boost::optional<Variant> variant = m_step.getArgument(argument.name());
      if (!variant){
        return true;
      }
    }
  }

  return false;
}


void MeasureStepItem::remove()
{
  // if this step is being edited, clear the edit controller
  MeasureStepItem* measureStepItem = m_app->editController()->measureStepItem();
  if (measureStepItem){
    if (measureStepItem->measureStep() == m_step){
      m_app->editController()->reset();
    }
  }

  qobject_cast<MeasureStepController *>(controller())->removeItemForStep(m_step);
}

void MeasureStepItem::moveUp()
{
  qobject_cast<MeasureStepController *>(controller())->moveUp(m_step);
}

void MeasureStepItem::moveDown()
{
  qobject_cast<MeasureStepController *>(controller())->moveDown(m_step);
}

void MeasureStepItem::setName(const QString & name)
{
  m_step.setName(name.toStdString());

  emit nameChanged(name);
}

void MeasureStepItem::setDisplayName(const QString & displayName)
{
  m_step.setName(displayName.toStdString());

  emit displayNameChanged(displayName);
}

void MeasureStepItem::setDescription(const QString & description)
{
  m_step.setDescription(description.toStdString());
}


void MeasureStepItem::setArgument(const measure::OSArgument& argument)
{
  if (argument.hasValue()){

    if (argument.type() == measure::OSArgumentType::Boolean){
      m_step.setArgument(argument.name(), argument.valueAsBool());
    }else if (argument.type() == measure::OSArgumentType::Double){
      m_step.setArgument(argument.name(), argument.valueAsDouble());
    }else if (argument.type() == measure::OSArgumentType::Quantity){
      m_step.setArgument(argument.name(), argument.valueAsDouble());
    }else if (argument.type() == measure::OSArgumentType::Integer){
      m_step.setArgument(argument.name(), argument.valueAsInteger());
    } else{
      m_step.setArgument(argument.name(), argument.valueAsString());
    }
  } else{
    m_step.removeArgument(argument.name());
  }

  emit argumentsChanged(hasIncompleteArguments());
}

void MeasureStepItem::setSelected(bool isSelected)
{
  OSListItem::setSelected(isSelected);

  // Ugly hack to prevent the controller from doing this for tab 2.
  // Please somebody fix, perhaps be using a new signal from OSItemSelectionController.
  if (!controller()->selectionController()->allowMultipleSelections())
  {
    if (isSelected)
    {
      m_app->chooseHorizontalEditTab();
      m_app->editController()->setMeasureStepItem(this, m_app);

    } else
    {
      m_app->editController()->reset();
    }
  }
}

MeasureStepItemDelegate::MeasureStepItemDelegate()
{}

QWidget * MeasureStepItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<MeasureStepItem> measureStepItem = dataSource.objectCast<MeasureStepItem>())
  {
    auto workflowStepView = new WorkflowStepView();
    workflowStepView->workflowStepButton->nameLabel->setText(measureStepItem->displayName());

    connect(measureStepItem.data(), &MeasureStepItem::displayNameChanged, workflowStepView->workflowStepButton->nameLabel, &QLabel::setText);

    // Remove

    connect(workflowStepView->removeButton, &QPushButton::clicked, measureStepItem.data(), &MeasureStepItem::remove);

    // Selection

    workflowStepView->workflowStepButton->setHasEmphasis(measureStepItem->isSelected());

    connect(workflowStepView->workflowStepButton, &WorkflowStepButton::clicked, measureStepItem.data(), &MeasureStepItem::toggleSelected);

    connect(measureStepItem.data(), &MeasureStepItem::selectedChanged, workflowStepView->workflowStepButton, &WorkflowStepButton::setHasEmphasis);

    // Warning Icon

    workflowStepView->workflowStepButton->cautionLabel->setVisible(measureStepItem->hasIncompleteArguments());

    connect(measureStepItem.data(), &MeasureStepItem::argumentsChanged, workflowStepView->workflowStepButton->cautionLabel, &QLabel::setVisible);

    // Up and down buttons

    connect(workflowStepView->upButton, &QPushButton::clicked, measureStepItem.data(), &MeasureStepItem::moveUp);
    
    connect(workflowStepView->downButton, &QPushButton::clicked, measureStepItem.data(), &MeasureStepItem::moveDown);
    


    return workflowStepView;
  }

  return new QWidget();
}


} // measuretab


} // openstudio

