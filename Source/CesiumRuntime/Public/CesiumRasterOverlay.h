// Copyright 2020-2024 CesiumGS, Inc. and Contributors

#pragma once

#include "CesiumRasterOverlayLoadFailureDetails.h"
#include "CesiumRasterOverlays/RasterOverlay.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Engine/Texture.h"
#include "Engine/TextureDefines.h"
#include <memory>
#include "CesiumRasterOverlay.generated.h"

namespace Cesium3DTilesSelection {
class Tileset;
}

/**
 * The delegate for OnCesiumRasterOverlayLoadFailure, which is triggered when
 * the raster overlay encounters a load error.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(
    FCesiumRasterOverlayLoadFailure,
    const FCesiumRasterOverlayLoadFailureDetails&);

CESIUMRUNTIME_API extern FCesiumRasterOverlayLoadFailure
    OnCesiumRasterOverlayLoadFailure;

/**
 * This struct is passed through the raster overlay options and is used when
 * `prepareRasterInLoadThread` is called.
 */
USTRUCT(BlueprintType)
struct FRasterOverlayRendererOptions {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium")
  TEnumAsByte<TextureFilter> filter = TextureFilter::TF_Default;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium")
  TEnumAsByte<TextureGroup> group = TextureGroup::TEXTUREGROUP_World;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium")
  bool useMipmaps = true;
};

/**
 * A quadtree pyramid of 2D raster images meant to be draped over a Cesium 3D
 * Tileset. Raster overlays are commonly used for satellite imagery, street
 * maps, and more.
 */
UCLASS(Abstract)
class CESIUMRUNTIME_API UCesiumRasterOverlay : public UActorComponent {
  GENERATED_BODY()

public:
  /**
   * The key to use to match this overlay to a material layer.
   *
   * When using Material Layers, any material layers inside a "Cesium" layer
   * stack with a name that matches this name will have their Texture,
   * TranslationScale, and TextureCoordinateIndex properties set automatically
   * so that a ML_CesiumOverlay layer function (or similar) will correctly
   * sample from this overlay.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium")
  FString MaterialLayerKey = "Overlay0";

  /**
   * Sets the texture filter and texture group of raster tile images. Depending
   * on the project settings, the default texture filter, TF_Default, should
   * have the best quality.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium")
  FRasterOverlayRendererOptions rendererOptions;

  // Sets default values for this component's properties
  UCesiumRasterOverlay();

  /**
   * Displays this raster overlay on its owning Cesium 3D Tileset Actor, without
   * changing its activation state. It is usually better to call Activate
   * rather than this function, in order to ensure that the component is also
   * activated. Otherwise, if the Cesium3DTileset is reloaded for any reason,
   * this overlay will no longer be shown.
   *
   * If the overlay is already added or if this component's Owner is not a
   * Cesium 3D Tileset, this method does nothing.
   */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void AddToTileset();

  /**
   * Stops displaying this raster overlay on its owning Cesium 3D Tileset Actor.
   * It is usually better to call Deactivate rather than this function, in order
   * to ensure that the component is also deactivated. Otherwise, if the
   * component remains active and the Cesium3DTileset is reloaded for any
   * reason, this overlay will reappear.
   *
   * If the overlay is not yet added or if this component's Owner is not a
   * Cesium 3D Tileset, this method does nothing.
   */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void RemoveFromTileset();

  /**
   * Refreshes this overlay by removing from its owning Cesium 3D Tileset Actor
   * and re-adding it. If this component's Owner is not a Cesium 3D Tileset
   * Actor, this method does nothing. If this component is not active, the
   * overlay will be removed from the Cesium3DTileset if already present but not
   * re-added.
   */
  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void Refresh();

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  double GetMaximumScreenSpaceError() const;

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetMaximumScreenSpaceError(double Value);

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  int32 GetMaximumTextureSize() const;

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetMaximumTextureSize(int32 Value);

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  int32 GetMaximumSimultaneousTileLoads() const;

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetMaximumSimultaneousTileLoads(int32 Value);

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  int64 GetSubTileCacheBytes() const;

  UFUNCTION(BlueprintCallable, Category = "Cesium")
  void SetSubTileCacheBytes(int64 Value);

  /**
   * Activates this raster overlay, which will add it to (that is: display it
   * on) the Cesium3DTileset to which the component is attached, if it hasn't
   * already been added. The overlay will continue to be shown on the tileset
   * until it is deactivated.
   *
   * If the overlay is already displayed on the Cesium3DTileset, calling this
   * function will not cause it to pick up any new values for properties that
   * have been modified since it as added. To do that, call Refresh.
   *
   * If you created this overlay component via Blueprints, consider setting the
   * "Auto Activate" property to false on the "Add Component" node and call
   * Activate after setting all the desired properties. This will avoid the need
   * to call Refresh, and will ensure the overlay is not loaded multiple times.
   *
   * @param bReset  Whether the activation should happen even if ShouldActivate
   * returns false.
   */
  virtual void Activate(bool bReset) override;

  /**
   * Deactivates this raster overlay. This will remove it from (that is: stop
   * displaying it on) the Cesium3DTileset to which the component is attached.
   * The overlay will not be shown again until the component is re-activated.
   */
  virtual void Deactivate() override;

  virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
  virtual bool IsReadyForFinishDestroy() override;

protected:
  /**
   * The maximum number of pixels of error when rendering this overlay.
   * This is used to select an appropriate level-of-detail.
   *
   * When this property has its default value, 2.0, it means that raster overlay
   * images will be sized so that, when zoomed in closest, a single pixel in
   * the raster overlay maps to approximately 2x2 pixels on the screen.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      BlueprintGetter = GetMaximumScreenSpaceError,
      BlueprintSetter = SetMaximumScreenSpaceError,
      Category = "Cesium")
  double MaximumScreenSpaceError = 2.0;

  /**
   * The maximum texel size of raster overlay textures, in either
   * direction.
   *
   * Images created by this overlay will be no more than this number of texels
   * in either direction. This may result in reduced raster overlay detail in
   * some cases.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      BlueprintGetter = GetMaximumTextureSize,
      BlueprintSetter = SetMaximumTextureSize,
      Category = "Cesium")
  int32 MaximumTextureSize = 2048;

  /**
   * The maximum number of overlay tiles that may simultaneously be in
   * the process of loading.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      BlueprintGetter = GetMaximumSimultaneousTileLoads,
      BlueprintSetter = SetMaximumSimultaneousTileLoads,
      Category = "Cesium")
  int32 MaximumSimultaneousTileLoads = 20;

  /**
   * The maximum number of bytes to use to cache sub-tiles in memory.
   *
   * This is used by provider types, that have an underlying tiling
   * scheme that may not align with the tiling scheme of the geometry tiles on
   * which the raster overlay tiles are draped. Because a single sub-tile may
   * overlap multiple geometry tiles, it is useful to cache loaded sub-tiles
   * in memory in case they're needed again soon. This property controls the
   * maximum size of that cache.
   */
  UPROPERTY(
      EditAnywhere,
      BlueprintReadWrite,
      BlueprintGetter = GetSubTileCacheBytes,
      BlueprintSetter = SetSubTileCacheBytes,
      Category = "Cesium")
  int64 SubTileCacheBytes = 16 * 1024 * 1024;

  /**
   * Whether or not to show credits of this raster overlay on screen.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cesium")
  bool ShowCreditsOnScreen;

#if WITH_EDITOR
  // Called when properties are changed in the editor
  virtual void
  PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

  Cesium3DTilesSelection::Tileset* FindTileset() const;

  virtual std::unique_ptr<CesiumRasterOverlays::RasterOverlay>
  CreateOverlay(const CesiumRasterOverlays::RasterOverlayOptions& options = {})
      PURE_VIRTUAL(UCesiumRasterOverlay::CreateOverlay, return nullptr;);

  virtual void OnAdd(
      Cesium3DTilesSelection::Tileset* pTileset,
      CesiumRasterOverlays::RasterOverlay* pOverlay) {}
  virtual void OnRemove(
      Cesium3DTilesSelection::Tileset* pTileset,
      CesiumRasterOverlays::RasterOverlay* pOverlay) {}

private:
  CesiumRasterOverlays::RasterOverlay* _pOverlay;
  int32 _overlaysBeingDestroyed;
};
