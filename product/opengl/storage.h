#pragma once
#include "base.h"
#include <map>
#include <memory>

namespace cxx {

namespace gl {

    class Storage : public Base {
    public:
        Storage(const std::string& id)
            : Base(Base::STORAGE, id) {};

        template <typename T, typename... Args>
        std::shared_ptr<T> put(std::string id, Args&&... args)
        {
            if (items.find(id) != items.end())
                LOG(WARNING) << "name conflict, already have " << *items[id] << " in " << *this;
            auto item = std::make_shared<T>(id, args...);
            items[id] = item;
            return item;
        }

        template <typename T>
        std::shared_ptr<T> get(const std::string& id) const
        {
            if (items.find(id) == items.end()) {
                LOG(ERROR) << "can't find item id=" << QUOT(id) << " in " << *this;
                exit(-1);
            }
            return std::dynamic_pointer_cast<T>(items.at(id));
        }

    private:
        std::map<std::string, std::shared_ptr<Base>> items;
    };

} // namespace gl

} // namespace cxx