#pragma once

#include <unordered_set>

#include "../glm.h"
#include "../shared_ptr.h"

namespace scene {

DECL_SHARED(Node)

class Node {
 public:
  Node() {}

  void add(const NodePtr& node) { children.insert(node); }

  void remove(const NodePtr& node) { children.erase(node); }

  const glm::mat4& transform() const { return matrix; }

 private:
  std::unordered_set<const NodePtr> children;

  glm::mat4 matrix;
};

}  // namespace scene
