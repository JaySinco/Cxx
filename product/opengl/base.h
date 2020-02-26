#pragma once
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#define QUOT(x) (std::string("\"")+x+"\"")
#define STR(x) (std::string(x))

namespace cxx {

namespace gl {

class Base {
public:
    enum TYPE { 
        CAMERA,
        LIGHT,
        MATERIAL,
        MODEL,
        OBJECT,
        SCENERY,
        SHADER,
        STORAGE,
        TEXTURE,
     };
    TYPE getType() const { return type_; }
    std::string getId() const { return id_; }
protected:
    Base(TYPE type, const std::string &id): type_(type), id_(id) {};
    virtual ~Base() { LOG(INFO) << *this << " deleted"; }
private:
    // noncopyable
    Base(const Base&);
    const Base &operator=(const Base&);
    TYPE type_;
    std::string id_;

    friend std::ostream &operator<<(std::ostream &out, const Base &base) {
    switch (base.getType()) {
        case Base::CAMERA  : out << "camera"  ; break;
        case Base::LIGHT   : out << "light"   ; break;
        case Base::MATERIAL: out << "material"; break;
        case Base::MODEL   : out << "model"   ; break; 
        case Base::OBJECT  : out << "object"  ; break;
        case Base::SCENERY : out << "scenery" ; break;
        case Base::SHADER  : out << "shader"  ; break;
        case Base::STORAGE : out << "storage" ; break;
        case Base::TEXTURE : out << "texture" ; break;
    }
    out << "[id="  << QUOT(base.getId()) << "]";
    return out;
    }
};



} // namespace gl

} // namespace cxx