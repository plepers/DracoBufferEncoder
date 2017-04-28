// ================ SOURCE

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "awdbuf.h"

#include <iostream>
#include <fstream>

awdbuf_t::awdbuf_t(kaitai::kstream *p_io, kaitai::kstruct *p_parent, awdbuf_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = this;
    m_header = new header_t(m__io, this, m__root);
    int l_attributes = header()->num_attribs();
    m_attributes = new std::vector<attribute_t*>();
    m_attributes->reserve(l_attributes);
    for (int i = 0; i < l_attributes; i++) {
        m_attributes->push_back(new attribute_t(m__io, this, m__root));
    }
    m_padding = m__io->read_u1();
    int l_padbytes = padding();
    m_padbytes = new std::vector<uint8_t>();
    m_padbytes->reserve(l_padbytes);
    for (int i = 0; i < l_padbytes; i++) {
        m_padbytes->push_back(m__io->read_u1());
    }
    m_datasize = m__io->read_u4le();
    m_data = m__io->read_bytes(datasize());
}

awdbuf_t::~awdbuf_t() {
    delete m_header;
    for (std::vector<attribute_t*>::iterator it = m_attributes->begin(); it != m_attributes->end(); ++it) {
        delete *it;
    }
    delete m_attributes;
    delete m_padbytes;
}

awdbuf_t::header_t::header_t(kaitai::kstream *p_io, awdbuf_t *p_parent, awdbuf_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_num_attribs = m__io->read_u1();
    m_gl_type = m__io->read_u2le();
}

awdbuf_t::header_t::~header_t() {
}

awdbuf_t::attribute_t::attribute_t(kaitai::kstream *p_io, awdbuf_t *p_parent, awdbuf_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_name = new awdstring_t(m__io, this, m__root);
    m_bytesize = m__io->read_u1();
    m_gltype = m__io->read_u2le();
    m_flags = m__io->read_u1();
}

awdbuf_t::attribute_t::~attribute_t() {
    delete m_name;
}

awdbuf_t::awdstring_t::awdstring_t(kaitai::kstream *p_io, awdbuf_t::attribute_t *p_parent, awdbuf_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_len = m__io->read_u2le();
    m_value = kaitai::kstream::bytes_to_str(m__io->read_bytes(len()), std::string("UTF-8"));
}

awdbuf_t::awdstring_t::~awdstring_t() {
}
