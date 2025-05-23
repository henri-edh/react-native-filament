#include "RNFFilamentAssetWrapper.h"
#include "RNFAnimatorWrapper.h"
#include "RNFFilamentInstanceWrapper.h"
#include "RNFSceneWrapper.h"

#include <utils/Entity.h>
#include <utils/EntityInstance.h>

namespace margelo {

using namespace utils;

void FilamentAssetWrapper::loadHybridMethods() {
  registerHybridMethod("getRoot", &FilamentAssetWrapper::getRoot, this);
  registerHybridMethod("releaseSourceData", &FilamentAssetWrapper::releaseSourceData, this);
  registerHybridMethod("createAnimator", &FilamentAssetWrapper::createAnimator, this);
  registerHybridGetter("entityCount", &FilamentAssetWrapper::getEntityCount, this);
  registerHybridMethod("getEntities", &FilamentAssetWrapper::getEntities, this);
  registerHybridGetter("renderableEntityCount", &FilamentAssetWrapper::getRenderableEntityCount, this);
  registerHybridMethod("getRenderableEntities", &FilamentAssetWrapper::getRenderableEntities, this);
  registerHybridMethod("getBoundingBox", &FilamentAssetWrapper::getBoundingBox, this);
  registerHybridMethod("getFirstEntityByName", &FilamentAssetWrapper::getFirstEntityByName, this);
  registerHybridMethod("getInstance", &FilamentAssetWrapper::getInstance, this);
  registerHybridMethod("getAssetInstances", &FilamentAssetWrapper::getAssetInstances, this);
  registerHybridMethod("getMorphTargetNameAt", &FilamentAssetWrapper::getMorphTargetNameAt, this);
  registerHybridMethod("getMorphTargetCountAt", &FilamentAssetWrapper::getMorphTargetCountAt, this);
}

std::shared_ptr<EntityWrapper> FilamentAssetWrapper::getRoot() {
  Entity rootEntity = pointee()->getRoot();
  return std::make_shared<EntityWrapper>(rootEntity);
}

void FilamentAssetWrapper::releaseSourceData() {
  std::unique_lock lock(_mutex);
  pointee()->releaseSourceData();
}

std::shared_ptr<AnimatorWrapper>
FilamentAssetWrapper::createAnimator(std::shared_ptr<NameComponentManagerWrapper> nameComponentManagerWrapper) {
  FilamentInstance* instance = pointee()->getInstance();
  Animator* animator = instance->getAnimator();
  std::shared_ptr<NameComponentManager> manager = nameComponentManagerWrapper->getManager();
  return std::make_shared<AnimatorWrapper>(animator, instance, manager);
}

std::vector<std::shared_ptr<EntityWrapper>> FilamentAssetWrapper::getEntities() {
  std::vector<std::shared_ptr<EntityWrapper>> entities;
  const Entity* entityArray = pointee()->getEntities();
  for (int i = 0; i < pointee()->getEntityCount(); i++) {
    entities.push_back(std::make_shared<EntityWrapper>(entityArray[i]));
  }
  return entities;
}

std::vector<std::shared_ptr<EntityWrapper>> FilamentAssetWrapper::getRenderableEntities() {
  std::vector<std::shared_ptr<EntityWrapper>> entities;
  const Entity* entityArray = pointee()->getRenderableEntities();
  for (int i = 0; i < pointee()->getRenderableEntityCount(); i++) {
    entities.push_back(std::make_shared<EntityWrapper>(entityArray[i]));
  }
  return entities;
}

std::optional<std::shared_ptr<EntityWrapper>> FilamentAssetWrapper::getFirstEntityByName(const std::string& name) {
  Entity entity = pointee()->getFirstEntityByName(name.c_str());
  if (entity.isNull()) {
    Logger::log("FilamentAssetWrapper", "Entity with name %s not found!", name.c_str());
    return std::nullopt;
  }

  return std::make_shared<EntityWrapper>(entity);
}

std::shared_ptr<FilamentInstanceWrapper> FilamentAssetWrapper::getInstance() {
  FilamentInstance* instance = pointee()->getInstance();
  return std::make_shared<FilamentInstanceWrapper>(instance);
}

std::vector<std::shared_ptr<FilamentInstanceWrapper>> FilamentAssetWrapper::getAssetInstances() {
  std::vector<std::shared_ptr<FilamentInstanceWrapper>> instances;
  FilamentInstance** instanceArray = pointee()->getAssetInstances();
  size_t instanceCount = pointee()->getAssetInstanceCount();
  for (int i = 0; i < instanceCount; i++) {
    instances.push_back(std::make_shared<FilamentInstanceWrapper>(instanceArray[i]));
  }
  return instances;
}

std::string FilamentAssetWrapper::getMorphTargetNameAt(const std::shared_ptr<EntityWrapper>& entity, int32_t targetIndex) {
  const Entity instance = entity->getEntity();
  const char* name = pointee()->getMorphTargetNameAt(instance, static_cast<size_t>(targetIndex));
  return std::string(name);
}

int32_t FilamentAssetWrapper::getMorphTargetCountAt(const std::shared_ptr<EntityWrapper>& entity) {
  const Entity instance = entity->getEntity();
  return pointee()->getMorphTargetCountAt(entity->getEntity());
}

} // namespace margelo
