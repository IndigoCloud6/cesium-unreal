// Copyright 2020-2024 CesiumGS, Inc. and Contributors

#include "CesiumTileMapServiceRasterOverlay.h"
#include "CesiumRasterOverlays/TileMapServiceRasterOverlay.h"
#include "CesiumRuntime.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

std::unique_ptr<CesiumRasterOverlays::RasterOverlay>
UCesiumTileMapServiceRasterOverlay::CreateOverlay(
    const CesiumRasterOverlays::RasterOverlayOptions& options) {
  if (this->Url.IsEmpty()) {
    // Don't create an overlay with an empty URL.
    return nullptr;
  }

  CesiumRasterOverlays::TileMapServiceRasterOverlayOptions tmsOptions;
  if (MaximumLevel > MinimumLevel && bSpecifyZoomLevels) {
    tmsOptions.minimumLevel = MinimumLevel;
    tmsOptions.maximumLevel = MaximumLevel;
  }
  return std::make_unique<CesiumRasterOverlays::TileMapServiceRasterOverlay>(
      TCHAR_TO_UTF8(*this->MaterialLayerKey),
      TCHAR_TO_UTF8(*this->Url),
      std::vector<CesiumAsync::IAssetAccessor::THeader>(),
      tmsOptions,
      options);
}

bool UCesiumTileMapServiceRasterOverlay::SetProperties(
    const FString& PropertiesJsonString) {
  TSharedPtr<FJsonObject> PropertiesJson;
  TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(PropertiesJsonString);
  if (FJsonSerializer::Deserialize(Reader, PropertiesJson) &&
      PropertiesJson.IsValid()) {
    FString StringValue;
    bool BoolValue;
    double DoubleValue;
    int64 IntAValue;
    int32 IntBValue;
    if (PropertiesJson->TryGetStringField(TEXT("Url"), StringValue)) {
      this->Url = StringValue;
    }
    if (PropertiesJson->TryGetBoolField(
            TEXT("bSpecifyZoomLevels"),
            BoolValue)) {
      this->bSpecifyZoomLevels = BoolValue;
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

FString UCesiumTileMapServiceRasterOverlay::GetProperties() {
  TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
  // Source
  JsonObject->SetStringField(TEXT("Url"), this->Url);
  JsonObject->SetBoolField(
      TEXT("bSpecifyZoomLevels"),
      this->bSpecifyZoomLevels);
  JsonObject->SetNumberField(TEXT("MaximumLevel"), this->MaximumLevel);
  JsonObject->SetNumberField(TEXT("MinimumLevel"), this->MinimumLevel);
  JsonObject->SetStringField(TEXT("MaterialLayerKey"), this->MaterialLayerKey);
  JsonObject->SetNumberField(
      TEXT("MaximumTextureSize"),
      this->MaximumTextureSize);
  JsonObject->SetNumberField(
      TEXT("MaximumScreenSpaceError"),
      this->MaximumScreenSpaceError);
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
  return TEXT("");
}

bool UCesiumTileMapServiceRasterOverlay::SetAttributes(
    const FString& Attributes) {
  TSharedPtr<FJsonObject> AttributesJson;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Attributes);
  if (FJsonSerializer::Deserialize(Reader, AttributesJson) &&
      AttributesJson.IsValid()) {
    this->bSpecifyZoomLevels = true;

    if (AttributesJson->HasField(TEXT("Url"))) {
      this->Url = AttributesJson->GetStringField(TEXT("Url"));
    }
    if (AttributesJson->HasField(TEXT("MaterialLayerKey"))) {
      this->MaterialLayerKey =
          AttributesJson->GetStringField(TEXT("MaterialLayerKey"));
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
