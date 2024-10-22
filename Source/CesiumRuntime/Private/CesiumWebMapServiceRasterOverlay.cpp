// Copyright 2020-2024 CesiumGS, Inc. and Contributors

#include "CesiumWebMapServiceRasterOverlay.h"
#include "Algo/Transform.h"
#include "CesiumRasterOverlays/WebMapServiceRasterOverlay.h"
#include "CesiumRuntime.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

std::unique_ptr<CesiumRasterOverlays::RasterOverlay>
UCesiumWebMapServiceRasterOverlay::CreateOverlay(
    const CesiumRasterOverlays::RasterOverlayOptions& options) {
  if (this->BaseUrl.IsEmpty()) {
    // Don't create an overlay with an empty base URL.
    return nullptr;
  }

  CesiumRasterOverlays::WebMapServiceRasterOverlayOptions wmsOptions;
  if (MaximumLevel > MinimumLevel) {
    wmsOptions.minimumLevel = MinimumLevel;
    wmsOptions.maximumLevel = MaximumLevel;
  }
  wmsOptions.layers = TCHAR_TO_UTF8(*Layers);
  wmsOptions.filter = TCHAR_TO_UTF8(*Filter);
  wmsOptions.tileWidth = TileWidth;
  wmsOptions.tileHeight = TileHeight;
  return std::make_unique<CesiumRasterOverlays::WebMapServiceRasterOverlay>(
      TCHAR_TO_UTF8(*this->MaterialLayerKey),
      TCHAR_TO_UTF8(*this->BaseUrl),
      std::vector<CesiumAsync::IAssetAccessor::THeader>(),
      wmsOptions,
      options);
}

FString UCesiumWebMapServiceRasterOverlay::GetProperties() {
  TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
  // Source
  JsonObject->SetStringField(TEXT("BaseUrl"), this->BaseUrl);
  JsonObject->SetStringField(TEXT("Layers"), this->Layers);
  JsonObject->SetNumberField(TEXT("TileWidth"), this->TileWidth);
  JsonObject->SetNumberField(TEXT("TileHeight"), this->TileHeight);
  JsonObject->SetNumberField(TEXT("MaximumLevel"), this->MaximumLevel);
  JsonObject->SetNumberField(TEXT("MinimumLevel"), this->MinimumLevel);
  JsonObject->SetStringField(TEXT("MaterialLayerKey"), this->MaterialLayerKey);
  JsonObject->SetStringField(TEXT("Filter"), this->Filter);
  JsonObject->SetNumberField(
      TEXT("MaximumScreenSpaceError"),
      this->MaximumScreenSpaceError);
  JsonObject->SetNumberField(
      TEXT("MaximumTextureSize"),
      this->MaximumTextureSize);
  JsonObject->SetNumberField(
      TEXT("MaximumSimultaneousTileLoads"),
      this->MaximumSimultaneousTileLoads);
  JsonObject->SetNumberField(
      TEXT("SubTileCacheBytes"),
      this->SubTileCacheBytes);
  FString OutputString;
  TSharedRef<TJsonWriter<>> Writer =
      TJsonWriterFactory<>::Create(&OutputString);
  if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer)) {
    return OutputString;
  }
  return TEXT("{}");
}

bool UCesiumWebMapServiceRasterOverlay::SetProperties(
    const FString& PropertiesJsonString) {
  TSharedPtr<FJsonObject> PropertiesJson;
  TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(PropertiesJsonString);
  if (FJsonSerializer::Deserialize(Reader, PropertiesJson) &&
      PropertiesJson.IsValid()) {
    FString StringValue;
    // bool BoolValue;
    double DoubleValue;
    int64 IntAValue;
    int32 IntBValue;
    if (PropertiesJson->TryGetStringField(TEXT("BaseUrl"), StringValue)) {
      this->BaseUrl = StringValue;
    }
    if (PropertiesJson->TryGetStringField(TEXT("Layers"), StringValue)) {
      this->Layers = StringValue;
    }
    if (PropertiesJson->TryGetStringField(TEXT("Filter"), StringValue)) {
      this->Filter = StringValue;
    }
    if (PropertiesJson->TryGetNumberField(TEXT("TileWidth"), IntBValue)) {
      this->TileWidth = IntBValue;
    }
    if (PropertiesJson->TryGetNumberField(TEXT("TileHeight"), IntBValue)) {
      this->TileHeight = IntBValue;
    }
    if (PropertiesJson->TryGetNumberField(TEXT("MaximumLevel"), IntBValue)) {
      this->MaximumLevel = IntBValue;
    }
    if (PropertiesJson->TryGetNumberField(TEXT("MinimumLevel"), IntBValue)) {
      this->MinimumLevel = IntBValue;
    }
    if (PropertiesJson->TryGetStringField(
            TEXT("MaterialLayerKey"),
            StringValue)) {
      this->MaterialLayerKey = StringValue;
    }
    if (PropertiesJson->TryGetNumberField(
            TEXT("MaximumTextureSize"),
            IntBValue)) {
      this->MaximumTextureSize = IntBValue;
    }
    if (PropertiesJson->TryGetNumberField(
            TEXT("MaximumScreenSpaceError"),
            DoubleValue)) {
      this->MaximumScreenSpaceError = DoubleValue;
    }
    if (PropertiesJson->TryGetNumberField(
            TEXT("MaximumSimultaneousTileLoads"),
            IntBValue)) {
      this->MaximumSimultaneousTileLoads = IntBValue;
    }
    if (PropertiesJson->TryGetNumberField(
            TEXT("SubTileCacheBytes"),
            IntAValue)) {
      this->SubTileCacheBytes = IntAValue;
    }
    this->Refresh();
  }
  return true;
}

bool UCesiumWebMapServiceRasterOverlay::SetAttributes(
    const FString& Attributes) {
  TSharedPtr<FJsonObject> AttributesJson;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Attributes);
  if (FJsonSerializer::Deserialize(Reader, AttributesJson) &&
      AttributesJson.IsValid()) {
    if (AttributesJson->HasField(TEXT("BaseUrl"))) {
      this->BaseUrl = AttributesJson->GetStringField(TEXT("BaseUrl"));
    }
    if (AttributesJson->HasField(TEXT("Layers"))) {
      this->Layers = AttributesJson->GetStringField(TEXT("Layers"));
    }
    if (AttributesJson->HasField(TEXT("Filter"))) {
      this->Filter = AttributesJson->GetStringField(TEXT("Filter"));
    }
    if (AttributesJson->HasField(TEXT("MaterialLayerKey"))) {
      this->MaterialLayerKey =
          AttributesJson->GetStringField(TEXT("MaterialLayerKey"));
    }
    if (AttributesJson->HasField(TEXT("TileHeight"))) {
      this->TileHeight = AttributesJson->GetNumberField(TEXT("TileHeight"));
    }
    if (AttributesJson->HasField(TEXT("TileWidth"))) {
      this->TileWidth = AttributesJson->GetNumberField(TEXT("TileWidth"));
    }
    if (AttributesJson->HasField(TEXT("MinimumLevel"))) {
      this->MinimumLevel = AttributesJson->GetNumberField(TEXT("MinimumLevel"));
    }
    if (AttributesJson->HasField(TEXT("MaximumLevel"))) {
      this->MaximumLevel = AttributesJson->GetNumberField(TEXT("MaximumLevel"));
    }
    if (AttributesJson->HasField(TEXT("MaximumSimultaneousTileLoads"))) {
      this->SetMaximumSimultaneousTileLoads(
          AttributesJson->GetNumberField(TEXT("MaximumSimultaneousTileLoads")));
    }
    if (AttributesJson->HasField(TEXT("MaximumScreenSpaceError"))) {
      this->SetMaximumScreenSpaceError(
          AttributesJson->GetNumberField(TEXT("MaximumScreenSpaceError")));
    }
    if (AttributesJson->HasField(TEXT("SubTileCacheBytes"))) {
      this->SetSubTileCacheBytes(
          AttributesJson->GetNumberField(TEXT("SubTileCacheBytes")));
    }
    if (AttributesJson->HasField(TEXT("MaximumTextureSize"))) {
      this->SetMaximumTextureSize(
          AttributesJson->GetNumberField(TEXT("MaximumTextureSize")));
    }
    this->Refresh();
    return true;
  }
  return false;
}
