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

#include "Json.hpp"

#include "Assert.hpp"
#include "Compare.hpp"
#include "Logger.hpp"
#include "PathHelpers.hpp"
#include "FilesystemHelpers.hpp"
#include "String.hpp"

#include <OpenStudio.hxx>

#include <QJsonDocument>
#include <QJsonObject>

namespace openstudio {

QVariant jsonMetadata() {
  QVariantMap metadata;
  metadata["openstudio_version"] = toQString(openStudioVersion());
  return metadata;
}

bool saveJSON(const QVariant& json, openstudio::path p, bool overwrite) {
  // Ensures file extension is .json. Warns if there is a mismatch.
  p = setFileExtension(p,"json",true);

  openstudio::filesystem::ofstream file(p, std::ios_base::binary);
  if (file.is_open()) {
    QJsonDocument doc = QJsonDocument::fromVariant(json);
    const auto json = doc.toJson();
    file.write(json.constData(), json.size());
    file.close();

    return true;
  }

  LOG_FREE(Error,"openstudio.Json","Could not open file " << toString(p) << " for writing.");
  return false;
}

std::string toJSON(const QVariant& json) {
  QJsonDocument doc = QJsonDocument::fromVariant(json);

  return toString(QString(doc.toJson()));
}

QVariant loadJSON(const openstudio::path& p) {
  openstudio::filesystem::ifstream file(p, std::ios_base::binary);
  if (file.is_open()) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(openstudio::filesystem::read_as_QByteArray(file), &err);
    file.close();
    if (err.error) {
      LOG_FREE_AND_THROW("openstudio.Json","Error parsing JSON: " + toString(err.errorString()));
    }
    return doc.toVariant();
  }

  LOG_FREE_AND_THROW("openstudio.Json","Could not open file " << toString(p) << " for reading.");
  return QVariant();
}

QVariant loadJSON(const std::string& json) {
  QJsonDocument doc = QJsonDocument::fromJson(toQString(json).toUtf8());
  if (doc.isNull()) {
    LOG_FREE_AND_THROW("openstudio.Json","Error parsing JSON");
  }
  return doc.toVariant();
}

VersionString extractOpenStudioVersion(const QVariant& variant) {
  QJsonObject topLevel = QJsonDocument::fromVariant(variant).object();
  if (topLevel.contains("metadata")) {
    topLevel = topLevel["metadata"].toObject();
  }
  
  OptionalVersionString version;
  if (topLevel.contains("openstudio_version")) {
    version = VersionString(topLevel["openstudio_version"].toString().toStdString());
  }
  else if (topLevel.contains("version")) {
    version = VersionString(topLevel["version"].toString().toStdString());
  }
  else {
    LOG_FREE_AND_THROW("openstudio.core.Json","No version identifier found in QJSON variant.");
  }
  OS_ASSERT(version);
  if (version.get() > VersionString(openStudioVersion())) {
    LOG_FREE(Warn,"openstudio.Json","Loading json file from version " << version
             << " with OpenStudio version " << VersionString(openStudioVersion())
             << ". OpenStudio json files are not designed to be forwards-compatible. "
             << "Unexpected behavior may result.")
  }

  return version.get();
}

} // openstudio
