#include "osutils.h"
namespace osutils {

std::string join(const std::string &parent, const std::string &child) {
  if (!parent.empty()) {
    if (parent.back() == pathSeparator())
      return parent + child;
    else {
      std::string new_parent{parent};
      new_parent.push_back(pathSeparator());
      return new_parent + child;
    }
  } else
    return join("./", child);
}

std::string join(const std::string &parent, const std::string &child1,
                 const std::string &child2) {
  return join(join(parent, child1), child2);
}
}
