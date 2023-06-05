// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "CesiumFeatureIdSet.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/ObjectMacros.h"

#include "CesiumPrimitiveFeatures.generated.h"

namespace CesiumGltf {
struct ExtensionExtMeshFeatures;
} // namespace CesiumGltf

/**
 * A Blueprint-accessible wrapper for a glTF Primitive's mesh features. It holds
 * views of the feature ID sets associated with this primitive.
 */
USTRUCT(BlueprintType)
struct CESIUMRUNTIME_API FCesiumPrimitiveFeatures {
  GENERATED_USTRUCT_BODY()

  using VertexIDAccessorType = std::variant<
      CesiumGltf::AccessorView<CesiumGltf::AccessorTypes::SCALAR<uint8_t>>,
      CesiumGltf::AccessorView<CesiumGltf::AccessorTypes::SCALAR<uint16_t>>,
      CesiumGltf::AccessorView<CesiumGltf::AccessorTypes::SCALAR<uint32_t>>>;

public:
  /**
   * Construct an empty primitive features.
   */
  FCesiumPrimitiveFeatures() {}

  /**
   * Constructs a primitive features instance.
   *
   * @param model The model that contains the EXT_mesh_features extension
   * @param primitive The mesh primitive that stores EXT_feature_metadata
   * extension
   * @param features The EXT_mesh_features of the gltf mesh primitive.
   * primitive
   */
  FCesiumPrimitiveFeatures(
      const CesiumGltf::Model& model,
      const CesiumGltf::MeshPrimitive& primitive,
      const CesiumGltf::ExtensionExtMeshFeatures& features);

private:
  TArray<FCesiumFeatureIDSet> _featureIDSets;
  VertexIDAccessorType _vertexIDAccessor;

  friend class UCesiumPrimitiveFeaturesBlueprintLibrary;
};

UCLASS()
class CESIUMRUNTIME_API UCesiumPrimitiveFeaturesBlueprintLibrary
    : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

public:
  /**
   * Gets all the feature ID sets that are associated with the
   * primitive.
   */
  UFUNCTION(
      BlueprintCallable,
      BlueprintPure,
      Category = "Cesium|Primitive|Features")
  static const TArray<FCesiumFeatureIDSet>&
  GetFeatureIDSets(UPARAM(ref)
                       const FCesiumPrimitiveFeatures& PrimitiveFeatures);

  /**
   * Gets all the feature ID sets of the given type. If the primitive has no
   * sets of that type, the returned array will be empty.
   */
  UFUNCTION(
      BlueprintCallable,
      BlueprintPure,
      Category = "Cesium|Primitive|Features")
  static const TArray<FCesiumFeatureIDSet> GetFeatureIDSetsOfType(
      UPARAM(ref) const FCesiumPrimitiveFeatures& PrimitiveFeatures,
      ECesiumFeatureIDType Type);

  /**
   * Gets the index of the first vertex that makes up a given face of this
   * primitive.
   *
   * @param FaceIndex The index of the face.
   */
  UFUNCTION(
      BlueprintCallable,
      BlueprintPure,
      Category = "Cesium|Primitive|Features")
  static int64 GetFirstVertexIndexFromFace(
      UPARAM(ref) const FCesiumPrimitiveFeatures& PrimitiveFeatures,
      int64 FaceIndex);

  /**
   * Gets the feature ID associated with the given face and feature ID set. If
   * the given feature ID set is a feature ID texture, this returns the feature ID
   * using the texture coordinates of the first vertex of the face.
   */
  UFUNCTION(
      BlueprintCallable,
      BlueprintPure,
      Category = "Cesium|Primitive|Features")
  static int64 GetFeatureIDFromFace(
      UPARAM(ref) const FCesiumPrimitiveFeatures& PrimitiveFeatures,
      UPARAM(ref) const FCesiumFeatureIDSet& FeatureIDSet,
      int64 FaceIndex);
};
