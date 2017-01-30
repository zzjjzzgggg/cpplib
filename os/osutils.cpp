#include "osutils.h"
namespace osutils {

std::string joinPath(const std::string &parent,
                     const std::string &child) {
  if (!parent.empty()) {
    if (parent.back() == pathSeparator())
      return parent + child;
    else {
      std::string new_parent{parent};
      new_parent.push_back(pathSeparator());
      return new_parent + child;
    }
  } else
    return joinPath("./", child);
}

}
