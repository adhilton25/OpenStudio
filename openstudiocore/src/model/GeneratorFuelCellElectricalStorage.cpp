/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "GeneratorFuelCellElectricalStorage.hpp"
#include "GeneratorFuelCellElectricalStorage_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_ElectricalStorage_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelCellElectricalStorage_Impl::GeneratorFuelCellElectricalStorage_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellElectricalStorage::iddObjectType());
  }

  GeneratorFuelCellElectricalStorage_Impl::GeneratorFuelCellElectricalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellElectricalStorage::iddObjectType());
  }

  GeneratorFuelCellElectricalStorage_Impl::GeneratorFuelCellElectricalStorage_Impl(const GeneratorFuelCellElectricalStorage_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellElectricalStorage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCellElectricalStorage_Impl::iddObjectType() const {
    return GeneratorFuelCellElectricalStorage::iddObjectType();
  }

  boost::optional<std::string> GeneratorFuelCellElectricalStorage_Impl::choiceofModel() const {
    return getString(OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel,true);
  }

  boost::optional<double> GeneratorFuelCellElectricalStorage_Impl::nominalChargingEnergeticEfficiency() const {
    return getDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalChargingEnergeticEfficiency,true);
  }

  boost::optional<double> GeneratorFuelCellElectricalStorage_Impl::nominalDischargingEnergeticEfficiency() const {
    return getDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalDischargingEnergeticEfficiency,true);
  }

  boost::optional<double> GeneratorFuelCellElectricalStorage_Impl::simpleMaximumCapacity() const {
    return getDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumCapacity,true);
  }

  boost::optional<double> GeneratorFuelCellElectricalStorage_Impl::simpleMaximumPowerDraw() const {
    return getDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerDraw,true);
  }

  boost::optional<double> GeneratorFuelCellElectricalStorage_Impl::simpleMaximumPowerStore() const {
    return getDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerStore,true);
  }

  boost::optional<double> GeneratorFuelCellElectricalStorage_Impl::initialChargeState() const {
    return getDouble(OS_Generator_FuelCell_ElectricalStorageFields::InitialChargeState,true);
  }

  bool GeneratorFuelCellElectricalStorage_Impl::setChoiceofModel(const std::string& choiceofModel) {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel, choiceofModel);
    return result;
  }

  void GeneratorFuelCellElectricalStorage_Impl::resetChoiceofModel() {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellElectricalStorage_Impl::setNominalChargingEnergeticEfficiency(double nominalChargingEnergeticEfficiency) {
    bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalChargingEnergeticEfficiency, nominalChargingEnergeticEfficiency);
    return result;
  }

  void GeneratorFuelCellElectricalStorage_Impl::resetNominalChargingEnergeticEfficiency() {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::NominalChargingEnergeticEfficiency, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellElectricalStorage_Impl::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
    bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalDischargingEnergeticEfficiency, nominalDischargingEnergeticEfficiency);
    return result;
  }

  void GeneratorFuelCellElectricalStorage_Impl::resetNominalDischargingEnergeticEfficiency() {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::NominalDischargingEnergeticEfficiency, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::setSimpleMaximumCapacity(double simpleMaximumCapacity) {
    bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumCapacity, simpleMaximumCapacity);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::resetSimpleMaximumCapacity() {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumCapacity, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::setSimpleMaximumPowerDraw(double simpleMaximumPowerDraw) {
    bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerDraw, simpleMaximumPowerDraw);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::resetSimpleMaximumPowerDraw() {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerDraw, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::setSimpleMaximumPowerStore(double simpleMaximumPowerStore) {
    bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerStore, simpleMaximumPowerStore);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::resetSimpleMaximumPowerStore() {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerStore, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::setInitialChargeState(double initialChargeState) {
    bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::InitialChargeState, initialChargeState);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellElectricalStorage_Impl::resetInitialChargeState() {
    bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::InitialChargeState, "");
    OS_ASSERT(result);
  }

} // detail

GeneratorFuelCellElectricalStorage::GeneratorFuelCellElectricalStorage(const Model& model)
  : ModelObject(GeneratorFuelCellElectricalStorage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType GeneratorFuelCellElectricalStorage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_ElectricalStorage);
}

std::vector<std::string> GeneratorFuelCellElectricalStorage::choiceofModelValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel);
}

boost::optional<std::string> GeneratorFuelCellElectricalStorage::choiceofModel() const {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->choiceofModel();
}

boost::optional<double> GeneratorFuelCellElectricalStorage::nominalChargingEnergeticEfficiency() const {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->nominalChargingEnergeticEfficiency();
}

boost::optional<double> GeneratorFuelCellElectricalStorage::nominalDischargingEnergeticEfficiency() const {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->nominalDischargingEnergeticEfficiency();
}

boost::optional<double> GeneratorFuelCellElectricalStorage::simpleMaximumCapacity() const {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->simpleMaximumCapacity();
}

boost::optional<double> GeneratorFuelCellElectricalStorage::simpleMaximumPowerDraw() const {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->simpleMaximumPowerDraw();
}

boost::optional<double> GeneratorFuelCellElectricalStorage::simpleMaximumPowerStore() const {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->simpleMaximumPowerStore();
}

boost::optional<double> GeneratorFuelCellElectricalStorage::initialChargeState() const {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->initialChargeState();
}

bool GeneratorFuelCellElectricalStorage::setChoiceofModel(const std::string& choiceofModel) {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setChoiceofModel(choiceofModel);
}

void GeneratorFuelCellElectricalStorage::resetChoiceofModel() {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetChoiceofModel();
}

bool GeneratorFuelCellElectricalStorage::setNominalChargingEnergeticEfficiency(double nominalChargingEnergeticEfficiency) {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setNominalChargingEnergeticEfficiency(nominalChargingEnergeticEfficiency);
}

void GeneratorFuelCellElectricalStorage::resetNominalChargingEnergeticEfficiency() {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetNominalChargingEnergeticEfficiency();
}

bool GeneratorFuelCellElectricalStorage::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
  return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setNominalDischargingEnergeticEfficiency(nominalDischargingEnergeticEfficiency);
}

void GeneratorFuelCellElectricalStorage::resetNominalDischargingEnergeticEfficiency() {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetNominalDischargingEnergeticEfficiency();
}

void GeneratorFuelCellElectricalStorage::setSimpleMaximumCapacity(double simpleMaximumCapacity) {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setSimpleMaximumCapacity(simpleMaximumCapacity);
}

void GeneratorFuelCellElectricalStorage::resetSimpleMaximumCapacity() {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetSimpleMaximumCapacity();
}

void GeneratorFuelCellElectricalStorage::setSimpleMaximumPowerDraw(double simpleMaximumPowerDraw) {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setSimpleMaximumPowerDraw(simpleMaximumPowerDraw);
}

void GeneratorFuelCellElectricalStorage::resetSimpleMaximumPowerDraw() {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetSimpleMaximumPowerDraw();
}

void GeneratorFuelCellElectricalStorage::setSimpleMaximumPowerStore(double simpleMaximumPowerStore) {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setSimpleMaximumPowerStore(simpleMaximumPowerStore);
}

void GeneratorFuelCellElectricalStorage::resetSimpleMaximumPowerStore() {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetSimpleMaximumPowerStore();
}

void GeneratorFuelCellElectricalStorage::setInitialChargeState(double initialChargeState) {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setInitialChargeState(initialChargeState);
}

void GeneratorFuelCellElectricalStorage::resetInitialChargeState() {
  getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetInitialChargeState();
}

/// @cond
GeneratorFuelCellElectricalStorage::GeneratorFuelCellElectricalStorage(std::shared_ptr<detail::GeneratorFuelCellElectricalStorage_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
