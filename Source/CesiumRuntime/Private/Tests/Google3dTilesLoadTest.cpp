// Copyright 2020-2023 CesiumGS, Inc. and Contributors

#if WITH_EDITOR

#include "CesiumLoadTestCore.h"

#include "Engine/World.h"
#include "Misc/AutomationTest.h"

#include "Cesium3DTileset.h"
#include "CesiumSunSky.h"

using namespace Cesium;

namespace Cesium {
FString testGoogleUrl(
    "https://tile.googleapis.com/v1/3dtiles/root.json?key=AIzaSyCaIL-JEK2Tw9MMBVKSTIu8dPkwfzfqAbU");
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGoogleTilesPompidou,
    "Cesium.Performance.GoogleTiles.Pompidou",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::PerfFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGoogleTilesChrysler,
    "Cesium.Performance.GoogleTiles.Chrysler",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::PerfFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGoogleTilesGuggenheim,
    "Cesium.Performance.GoogleTiles.Guggenheim",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::PerfFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGoogleTilesDeathValley,
    "Cesium.Performance.GoogleTiles.DeathValley",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::PerfFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGoogleTilesTokyo,
    "Cesium.Performance.GoogleTiles.Tokyo",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::PerfFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGoogleTilesGoogleplex,
    "Cesium.Performance.GoogleTiles.Googleplex",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::PerfFilter)

#define TEST_SCREEN_WIDTH 1280
#define TEST_SCREEN_HEIGHT 720

void googleWarmCacheSetup(SceneGenerationContext& context) {
  context.refreshTilesets();
}

void setupForPompidou(SceneGenerationContext& context) {
  context.setCommonProperties(
      FVector(2.352200, 48.860600, 200),
      FVector(0, 0, 0),
      FRotator(-20.0, -90.0, 0.0),
      60.0f);

  context.sunSky->TimeZone = 2.0f;
  context.sunSky->UpdateSun();

  ACesium3DTileset* tileset = context.world->SpawnActor<ACesium3DTileset>();
  tileset->SetUrl(Cesium::testGoogleUrl);
  tileset->SetTilesetSource(ETilesetSource::FromUrl);
  tileset->SetActorLabel(TEXT("Center Pompidou, Paris, France"));
  context.tilesets.push_back(tileset);
}

void setupForChrysler(SceneGenerationContext& context) {
  context.setCommonProperties(
      FVector(-73.9752624659, 40.74697185903, 307.38),
      FVector(0, 0, 0),
      FRotator(-15.0, -90.0, 0.0),
      60.0f);

  context.sunSky->TimeZone = -4.0f;
  context.sunSky->UpdateSun();

  ACesium3DTileset* tileset = context.world->SpawnActor<ACesium3DTileset>();
  tileset->SetUrl(Cesium::testGoogleUrl);
  tileset->SetTilesetSource(ETilesetSource::FromUrl);
  tileset->SetActorLabel(TEXT("Chrysler Building, NYC"));
  context.tilesets.push_back(tileset);
}

void setupForGuggenheim(SceneGenerationContext& context) {
  context.setCommonProperties(
      FVector(-2.937, 43.2685, 150),
      FVector(0, 0, 0),
      FRotator(-15.0, 0.0, 0.0),
      60.0f);

  context.sunSky->TimeZone = 2.0f;
  context.sunSky->UpdateSun();

  ACesium3DTileset* tileset = context.world->SpawnActor<ACesium3DTileset>();
  tileset->SetUrl(Cesium::testGoogleUrl);
  tileset->SetTilesetSource(ETilesetSource::FromUrl);
  tileset->SetActorLabel(TEXT("Guggenheim Museum, Bilbao, Spain"));
  context.tilesets.push_back(tileset);
}

void setupForDeathValley(SceneGenerationContext& context) {
  context.setCommonProperties(
      FVector(-116.812278, 36.42, 300),
      FVector(0, 0, 0),
      FRotator(0, 0.0, 0.0),
      60.0f);

  context.sunSky->TimeZone = -7.0f;
  context.sunSky->UpdateSun();

  ACesium3DTileset* tileset = context.world->SpawnActor<ACesium3DTileset>();
  tileset->SetUrl(Cesium::testGoogleUrl);
  tileset->SetTilesetSource(ETilesetSource::FromUrl);
  tileset->SetActorLabel(
      TEXT("Zabriskie Point, Death Valley National Park, California"));
  context.tilesets.push_back(tileset);
}

void setupForTokyo(SceneGenerationContext& context) {
  context.setCommonProperties(
      FVector(139.7563178458, 35.652798383944, 525.62),
      FVector(0, 0, 0),
      FRotator(-15, -150, 0.0),
      60.0f);

  context.sunSky->TimeZone = 9.0f;
  context.sunSky->UpdateSun();

  ACesium3DTileset* tileset = context.world->SpawnActor<ACesium3DTileset>();
  tileset->SetUrl(Cesium::testGoogleUrl);
  tileset->SetTilesetSource(ETilesetSource::FromUrl);
  tileset->SetActorLabel(TEXT("Tokyo Tower, Tokyo, Japan"));
  context.tilesets.push_back(tileset);
}

void setupForGoogleplex(SceneGenerationContext& context) {
  context.setCommonProperties(
      FVector(-122.083969, 37.424492, 142.859116),
      FVector(0, 0, 0),
      FRotator(-25, 95, 0),
      90.0f);

  ACesium3DTileset* tileset = context.world->SpawnActor<ACesium3DTileset>();
  tileset->SetUrl(Cesium::testGoogleUrl);
  tileset->SetTilesetSource(ETilesetSource::FromUrl);
  tileset->SetActorLabel(TEXT("Google Photorealistic 3D Tiles"));

  context.tilesets.push_back(tileset);
}

bool FGoogleTilesPompidou::RunTest(const FString& Parameters) {
  std::vector<TestPass> testPasses;
  testPasses.push_back(TestPass{"Cold Cache", nullptr, nullptr});

  return RunLoadTest(
      GetBeautifiedTestName(),
      setupForPompidou,
      testPasses,
      TEST_SCREEN_WIDTH,
      TEST_SCREEN_HEIGHT);
}

bool FGoogleTilesChrysler::RunTest(const FString& Parameters) {
  std::vector<TestPass> testPasses;
  testPasses.push_back(TestPass{"Cold Cache", nullptr, nullptr});
  testPasses.push_back(TestPass{"Warm Cache", googleWarmCacheSetup, nullptr});

  return RunLoadTest(
      GetBeautifiedTestName(),
      setupForChrysler,
      testPasses,
      TEST_SCREEN_WIDTH,
      TEST_SCREEN_HEIGHT);
}

bool FGoogleTilesGuggenheim::RunTest(const FString& Parameters) {
  std::vector<TestPass> testPasses;
  testPasses.push_back(TestPass{"Cold Cache", nullptr, nullptr});
  testPasses.push_back(TestPass{"Warm Cache", googleWarmCacheSetup, nullptr});

  return RunLoadTest(
      GetBeautifiedTestName(),
      setupForGuggenheim,
      testPasses,
      TEST_SCREEN_WIDTH,
      TEST_SCREEN_HEIGHT);
}

bool FGoogleTilesDeathValley::RunTest(const FString& Parameters) {
  std::vector<TestPass> testPasses;
  testPasses.push_back(TestPass{"Cold Cache", nullptr, nullptr});
  testPasses.push_back(TestPass{"Warm Cache", googleWarmCacheSetup, nullptr});

  return RunLoadTest(
      GetBeautifiedTestName(),
      setupForDeathValley,
      testPasses,
      TEST_SCREEN_WIDTH,
      TEST_SCREEN_HEIGHT);
}

bool FGoogleTilesTokyo::RunTest(const FString& Parameters) {
  std::vector<TestPass> testPasses;
  testPasses.push_back(TestPass{"Cold Cache", nullptr, nullptr});
  testPasses.push_back(TestPass{"Warm Cache", googleWarmCacheSetup, nullptr});

  return RunLoadTest(
      GetBeautifiedTestName(),
      setupForTokyo,
      testPasses,
      TEST_SCREEN_WIDTH,
      TEST_SCREEN_HEIGHT);
}

bool FGoogleTilesGoogleplex::RunTest(const FString& Parameters) {
  std::vector<TestPass> testPasses;
  testPasses.push_back(TestPass{"Cold Cache", nullptr, nullptr});
  testPasses.push_back(TestPass{"Warm Cache", googleWarmCacheSetup, nullptr});

  return RunLoadTest(
      GetBeautifiedTestName(),
      setupForGoogleplex,
      testPasses,
      TEST_SCREEN_WIDTH,
      TEST_SCREEN_HEIGHT);
}

#endif
