// ================ HEADER

#ifndef AWDBUF_H_
#define AWDBUF_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include <kaitaistruct.h>
#include <kaitaistream.h>

#include <stdint.h>
#include <vector>
#include <sstream>

#if KAITAI_STRUCT_VERSION < 7000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.7 or later is required"
#endif

class awdbuf_t : public kaitai::kstruct {

public:
    class header_t;
    class attribute_t;
    class awdstring_t;

    awdbuf_t(kaitai::kstream* p_io, kaitai::kstruct* p_parent = 0, awdbuf_t* p_root = 0);
    ~awdbuf_t();

    class header_t : public kaitai::kstruct {

    public:

        header_t(kaitai::kstream* p_io, awdbuf_t* p_parent = 0, awdbuf_t* p_root = 0);
        ~header_t();

    private:
        uint8_t m_num_attribs;
        uint16_t m_gl_type;
        awdbuf_t* m__root;
        awdbuf_t* m__parent;

    public:
        uint8_t num_attribs() const { return m_num_attribs; }
        uint16_t gl_type() const { return m_gl_type; }
        awdbuf_t* _root() const { return m__root; }
        awdbuf_t* _parent() const { return m__parent; }
    };

    class attribute_t : public kaitai::kstruct {

    public:

        attribute_t(kaitai::kstream* p_io, awdbuf_t* p_parent = 0, awdbuf_t* p_root = 0);
        ~attribute_t();

    private:
        awdstring_t* m_name;
        uint8_t m_bytesize;
        uint16_t m_gltype;
        uint8_t m_flags;
        awdbuf_t* m__root;
        awdbuf_t* m__parent;

    public:
        awdstring_t* name() const { return m_name; }
        uint8_t bytesize() const { return m_bytesize; }
        uint16_t gltype() const { return m_gltype; }
        uint8_t flags() const { return m_flags; }
        awdbuf_t* _root() const { return m__root; }
        awdbuf_t* _parent() const { return m__parent; }
    };

    class awdstring_t : public kaitai::kstruct {

    public:

        awdstring_t(kaitai::kstream* p_io, awdbuf_t::attribute_t* p_parent = 0, awdbuf_t* p_root = 0);
        ~awdstring_t();

    private:
        uint16_t m_len;
        std::string m_value;
        awdbuf_t* m__root;
        awdbuf_t::attribute_t* m__parent;

    public:
        uint16_t len() const { return m_len; }
        std::string value() const { return m_value; }
        awdbuf_t* _root() const { return m__root; }
        awdbuf_t::attribute_t* _parent() const { return m__parent; }
    };

private:
    header_t* m_header;
    std::vector<attribute_t*>* m_attributes;
    uint8_t m_padding;
    std::vector<uint8_t>* m_padbytes;
    std::string m_data;
    awdbuf_t* m__root;
    kaitai::kstruct* m__parent;

public:
    header_t* header() const { return m_header; }
    std::vector<attribute_t*>* attributes() const { return m_attributes; }
    uint8_t padding() const { return m_padding; }
    std::vector<uint8_t>* padbytes() const { return m_padbytes; }
    std::string data() const { return m_data; }
    awdbuf_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // AWDBUF_H_
