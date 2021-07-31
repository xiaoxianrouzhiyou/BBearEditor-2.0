// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BBMaterial.proto

#include "BBMaterial.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace BBSerializer {
constexpr BBMaterial::BBMaterial(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : texturename_()
  , texturepath_()
  , floatname_()
  , floatvalue_()
  , vec4name_()
  , vec4value_()
  , shadername_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , vshaderpath_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , fshaderpath_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , cubemapname_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , cubemappaths_(nullptr)
  , blendstate_(false)
  , srcblendfunc_(0)
  , dstblendfunc_(0){}
struct BBMaterialDefaultTypeInternal {
  constexpr BBMaterialDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~BBMaterialDefaultTypeInternal() {}
  union {
    BBMaterial _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT BBMaterialDefaultTypeInternal _BBMaterial_default_instance_;
}  // namespace BBSerializer
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_BBMaterial_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_BBMaterial_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_BBMaterial_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_BBMaterial_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, shadername_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, vshaderpath_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, fshaderpath_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, texturename_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, texturepath_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, floatname_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, floatvalue_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, vec4name_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, vec4value_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, blendstate_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, srcblendfunc_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, dstblendfunc_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, cubemapname_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBMaterial, cubemappaths_),
  0,
  1,
  2,
  ~0u,
  ~0u,
  ~0u,
  ~0u,
  ~0u,
  ~0u,
  5,
  6,
  7,
  3,
  4,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 19, sizeof(::BBSerializer::BBMaterial)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::BBSerializer::_BBMaterial_default_instance_),
};

const char descriptor_table_protodef_BBMaterial_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020BBMaterial.proto\022\014BBSerializer\032\016BBVect"
  "or.proto\032\017BBCubeMap.proto\"\207\004\n\nBBMaterial"
  "\022\027\n\nshaderName\030\001 \001(\tH\000\210\001\001\022\030\n\013vShaderPath"
  "\030\002 \001(\tH\001\210\001\001\022\030\n\013fShaderPath\030\003 \001(\tH\002\210\001\001\022\023\n"
  "\013textureName\030\004 \003(\t\022\023\n\013texturePath\030\005 \003(\t\022"
  "\021\n\tfloatName\030\006 \003(\t\022\022\n\nfloatValue\030\007 \003(\002\022\020"
  "\n\010vec4Name\030\010 \003(\t\022+\n\tvec4Value\030\t \003(\0132\030.BB"
  "Serializer.BBVector4f\022\027\n\nblendState\030\n \001("
  "\010H\003\210\001\001\022\031\n\014SRCBlendFunc\030\013 \001(\005H\004\210\001\001\022\031\n\014DST"
  "BlendFunc\030\014 \001(\005H\005\210\001\001\022\030\n\013cubeMapName\030\r \001("
  "\tH\006\210\001\001\0222\n\014cubeMapPaths\030\016 \001(\0132\027.BBSeriali"
  "zer.BBCubeMapH\007\210\001\001B\r\n\013_shaderNameB\016\n\014_vS"
  "haderPathB\016\n\014_fShaderPathB\r\n\013_blendState"
  "B\017\n\r_SRCBlendFuncB\017\n\r_DSTBlendFuncB\016\n\014_c"
  "ubeMapNameB\017\n\r_cubeMapPathsb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_BBMaterial_2eproto_deps[2] = {
  &::descriptor_table_BBCubeMap_2eproto,
  &::descriptor_table_BBVector_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_BBMaterial_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_BBMaterial_2eproto = {
  false, false, 595, descriptor_table_protodef_BBMaterial_2eproto, "BBMaterial.proto", 
  &descriptor_table_BBMaterial_2eproto_once, descriptor_table_BBMaterial_2eproto_deps, 2, 1,
  schemas, file_default_instances, TableStruct_BBMaterial_2eproto::offsets,
  file_level_metadata_BBMaterial_2eproto, file_level_enum_descriptors_BBMaterial_2eproto, file_level_service_descriptors_BBMaterial_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_BBMaterial_2eproto_getter() {
  return &descriptor_table_BBMaterial_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_BBMaterial_2eproto(&descriptor_table_BBMaterial_2eproto);
namespace BBSerializer {

// ===================================================================

class BBMaterial::_Internal {
 public:
  using HasBits = decltype(std::declval<BBMaterial>()._has_bits_);
  static void set_has_shadername(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_vshaderpath(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_fshaderpath(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_blendstate(HasBits* has_bits) {
    (*has_bits)[0] |= 32u;
  }
  static void set_has_srcblendfunc(HasBits* has_bits) {
    (*has_bits)[0] |= 64u;
  }
  static void set_has_dstblendfunc(HasBits* has_bits) {
    (*has_bits)[0] |= 128u;
  }
  static void set_has_cubemapname(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static const ::BBSerializer::BBCubeMap& cubemappaths(const BBMaterial* msg);
  static void set_has_cubemappaths(HasBits* has_bits) {
    (*has_bits)[0] |= 16u;
  }
};

const ::BBSerializer::BBCubeMap&
BBMaterial::_Internal::cubemappaths(const BBMaterial* msg) {
  return *msg->cubemappaths_;
}
void BBMaterial::clear_vec4value() {
  vec4value_.Clear();
}
void BBMaterial::clear_cubemappaths() {
  if (GetArena() == nullptr && cubemappaths_ != nullptr) {
    delete cubemappaths_;
  }
  cubemappaths_ = nullptr;
  _has_bits_[0] &= ~0x00000010u;
}
BBMaterial::BBMaterial(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  texturename_(arena),
  texturepath_(arena),
  floatname_(arena),
  floatvalue_(arena),
  vec4name_(arena),
  vec4value_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:BBSerializer.BBMaterial)
}
BBMaterial::BBMaterial(const BBMaterial& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_),
      texturename_(from.texturename_),
      texturepath_(from.texturepath_),
      floatname_(from.floatname_),
      floatvalue_(from.floatvalue_),
      vec4name_(from.vec4name_),
      vec4value_(from.vec4value_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  shadername_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_shadername()) {
    shadername_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_shadername(), 
      GetArena());
  }
  vshaderpath_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_vshaderpath()) {
    vshaderpath_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_vshaderpath(), 
      GetArena());
  }
  fshaderpath_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_fshaderpath()) {
    fshaderpath_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_fshaderpath(), 
      GetArena());
  }
  cubemapname_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_cubemapname()) {
    cubemapname_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_cubemapname(), 
      GetArena());
  }
  if (from._internal_has_cubemappaths()) {
    cubemappaths_ = new ::BBSerializer::BBCubeMap(*from.cubemappaths_);
  } else {
    cubemappaths_ = nullptr;
  }
  ::memcpy(&blendstate_, &from.blendstate_,
    static_cast<size_t>(reinterpret_cast<char*>(&dstblendfunc_) -
    reinterpret_cast<char*>(&blendstate_)) + sizeof(dstblendfunc_));
  // @@protoc_insertion_point(copy_constructor:BBSerializer.BBMaterial)
}

void BBMaterial::SharedCtor() {
shadername_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
vshaderpath_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
fshaderpath_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
cubemapname_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&cubemappaths_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&dstblendfunc_) -
    reinterpret_cast<char*>(&cubemappaths_)) + sizeof(dstblendfunc_));
}

BBMaterial::~BBMaterial() {
  // @@protoc_insertion_point(destructor:BBSerializer.BBMaterial)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void BBMaterial::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  shadername_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  vshaderpath_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  fshaderpath_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  cubemapname_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete cubemappaths_;
}

void BBMaterial::ArenaDtor(void* object) {
  BBMaterial* _this = reinterpret_cast< BBMaterial* >(object);
  (void)_this;
}
void BBMaterial::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void BBMaterial::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void BBMaterial::Clear() {
// @@protoc_insertion_point(message_clear_start:BBSerializer.BBMaterial)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  texturename_.Clear();
  texturepath_.Clear();
  floatname_.Clear();
  floatvalue_.Clear();
  vec4name_.Clear();
  vec4value_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    if (cached_has_bits & 0x00000001u) {
      shadername_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      vshaderpath_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000004u) {
      fshaderpath_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000008u) {
      cubemapname_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000010u) {
      if (GetArena() == nullptr && cubemappaths_ != nullptr) {
        delete cubemappaths_;
      }
      cubemappaths_ = nullptr;
    }
  }
  if (cached_has_bits & 0x000000e0u) {
    ::memset(&blendstate_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&dstblendfunc_) -
        reinterpret_cast<char*>(&blendstate_)) + sizeof(dstblendfunc_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* BBMaterial::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string shaderName = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_shadername();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.shaderName"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string vShaderPath = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_vshaderpath();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.vShaderPath"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string fShaderPath = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_fshaderpath();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.fShaderPath"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated string textureName = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_texturename();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.textureName"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<34>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated string texturePath = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 42)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_texturepath();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.texturePath"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<42>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated string floatName = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 50)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_floatname();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.floatName"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<50>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated float floatValue = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 58)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedFloatParser(_internal_mutable_floatvalue(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 61) {
          _internal_add_floatvalue(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr));
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // repeated string vec4Name = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 66)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_vec4name();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.vec4Name"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<66>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated .BBSerializer.BBVector4f vec4Value = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 74)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_vec4value(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<74>(ptr));
        } else goto handle_unusual;
        continue;
      // bool blendState = 10;
      case 10:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 80)) {
          _Internal::set_has_blendstate(&has_bits);
          blendstate_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 SRCBlendFunc = 11;
      case 11:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 88)) {
          _Internal::set_has_srcblendfunc(&has_bits);
          srcblendfunc_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 DSTBlendFunc = 12;
      case 12:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 96)) {
          _Internal::set_has_dstblendfunc(&has_bits);
          dstblendfunc_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string cubeMapName = 13;
      case 13:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 106)) {
          auto str = _internal_mutable_cubemapname();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "BBSerializer.BBMaterial.cubeMapName"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .BBSerializer.BBCubeMap cubeMapPaths = 14;
      case 14:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 114)) {
          ptr = ctx->ParseMessage(_internal_mutable_cubemappaths(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* BBMaterial::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:BBSerializer.BBMaterial)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string shaderName = 1;
  if (_internal_has_shadername()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_shadername().data(), static_cast<int>(this->_internal_shadername().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.shaderName");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_shadername(), target);
  }

  // string vShaderPath = 2;
  if (_internal_has_vshaderpath()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_vshaderpath().data(), static_cast<int>(this->_internal_vshaderpath().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.vShaderPath");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_vshaderpath(), target);
  }

  // string fShaderPath = 3;
  if (_internal_has_fshaderpath()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_fshaderpath().data(), static_cast<int>(this->_internal_fshaderpath().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.fShaderPath");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_fshaderpath(), target);
  }

  // repeated string textureName = 4;
  for (int i = 0, n = this->_internal_texturename_size(); i < n; i++) {
    const auto& s = this->_internal_texturename(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.textureName");
    target = stream->WriteString(4, s, target);
  }

  // repeated string texturePath = 5;
  for (int i = 0, n = this->_internal_texturepath_size(); i < n; i++) {
    const auto& s = this->_internal_texturepath(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.texturePath");
    target = stream->WriteString(5, s, target);
  }

  // repeated string floatName = 6;
  for (int i = 0, n = this->_internal_floatname_size(); i < n; i++) {
    const auto& s = this->_internal_floatname(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.floatName");
    target = stream->WriteString(6, s, target);
  }

  // repeated float floatValue = 7;
  if (this->_internal_floatvalue_size() > 0) {
    target = stream->WriteFixedPacked(7, _internal_floatvalue(), target);
  }

  // repeated string vec4Name = 8;
  for (int i = 0, n = this->_internal_vec4name_size(); i < n; i++) {
    const auto& s = this->_internal_vec4name(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.vec4Name");
    target = stream->WriteString(8, s, target);
  }

  // repeated .BBSerializer.BBVector4f vec4Value = 9;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_vec4value_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(9, this->_internal_vec4value(i), target, stream);
  }

  // bool blendState = 10;
  if (_internal_has_blendstate()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(10, this->_internal_blendstate(), target);
  }

  // int32 SRCBlendFunc = 11;
  if (_internal_has_srcblendfunc()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(11, this->_internal_srcblendfunc(), target);
  }

  // int32 DSTBlendFunc = 12;
  if (_internal_has_dstblendfunc()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(12, this->_internal_dstblendfunc(), target);
  }

  // string cubeMapName = 13;
  if (_internal_has_cubemapname()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_cubemapname().data(), static_cast<int>(this->_internal_cubemapname().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "BBSerializer.BBMaterial.cubeMapName");
    target = stream->WriteStringMaybeAliased(
        13, this->_internal_cubemapname(), target);
  }

  // .BBSerializer.BBCubeMap cubeMapPaths = 14;
  if (_internal_has_cubemappaths()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        14, _Internal::cubemappaths(this), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:BBSerializer.BBMaterial)
  return target;
}

size_t BBMaterial::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:BBSerializer.BBMaterial)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated string textureName = 4;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(texturename_.size());
  for (int i = 0, n = texturename_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      texturename_.Get(i));
  }

  // repeated string texturePath = 5;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(texturepath_.size());
  for (int i = 0, n = texturepath_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      texturepath_.Get(i));
  }

  // repeated string floatName = 6;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(floatname_.size());
  for (int i = 0, n = floatname_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      floatname_.Get(i));
  }

  // repeated float floatValue = 7;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_floatvalue_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    total_size += data_size;
  }

  // repeated string vec4Name = 8;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(vec4name_.size());
  for (int i = 0, n = vec4name_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      vec4name_.Get(i));
  }

  // repeated .BBSerializer.BBVector4f vec4Value = 9;
  total_size += 1UL * this->_internal_vec4value_size();
  for (const auto& msg : this->vec4value_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x000000ffu) {
    // string shaderName = 1;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_shadername());
    }

    // string vShaderPath = 2;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_vshaderpath());
    }

    // string fShaderPath = 3;
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_fshaderpath());
    }

    // string cubeMapName = 13;
    if (cached_has_bits & 0x00000008u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_cubemapname());
    }

    // .BBSerializer.BBCubeMap cubeMapPaths = 14;
    if (cached_has_bits & 0x00000010u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
          *cubemappaths_);
    }

    // bool blendState = 10;
    if (cached_has_bits & 0x00000020u) {
      total_size += 1 + 1;
    }

    // int32 SRCBlendFunc = 11;
    if (cached_has_bits & 0x00000040u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
          this->_internal_srcblendfunc());
    }

    // int32 DSTBlendFunc = 12;
    if (cached_has_bits & 0x00000080u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
          this->_internal_dstblendfunc());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void BBMaterial::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:BBSerializer.BBMaterial)
  GOOGLE_DCHECK_NE(&from, this);
  const BBMaterial* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<BBMaterial>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:BBSerializer.BBMaterial)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:BBSerializer.BBMaterial)
    MergeFrom(*source);
  }
}

void BBMaterial::MergeFrom(const BBMaterial& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:BBSerializer.BBMaterial)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  texturename_.MergeFrom(from.texturename_);
  texturepath_.MergeFrom(from.texturepath_);
  floatname_.MergeFrom(from.floatname_);
  floatvalue_.MergeFrom(from.floatvalue_);
  vec4name_.MergeFrom(from.vec4name_);
  vec4value_.MergeFrom(from.vec4value_);
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x000000ffu) {
    if (cached_has_bits & 0x00000001u) {
      _internal_set_shadername(from._internal_shadername());
    }
    if (cached_has_bits & 0x00000002u) {
      _internal_set_vshaderpath(from._internal_vshaderpath());
    }
    if (cached_has_bits & 0x00000004u) {
      _internal_set_fshaderpath(from._internal_fshaderpath());
    }
    if (cached_has_bits & 0x00000008u) {
      _internal_set_cubemapname(from._internal_cubemapname());
    }
    if (cached_has_bits & 0x00000010u) {
      _internal_mutable_cubemappaths()->::BBSerializer::BBCubeMap::MergeFrom(from._internal_cubemappaths());
    }
    if (cached_has_bits & 0x00000020u) {
      blendstate_ = from.blendstate_;
    }
    if (cached_has_bits & 0x00000040u) {
      srcblendfunc_ = from.srcblendfunc_;
    }
    if (cached_has_bits & 0x00000080u) {
      dstblendfunc_ = from.dstblendfunc_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void BBMaterial::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:BBSerializer.BBMaterial)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BBMaterial::CopyFrom(const BBMaterial& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:BBSerializer.BBMaterial)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BBMaterial::IsInitialized() const {
  return true;
}

void BBMaterial::InternalSwap(BBMaterial* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  texturename_.InternalSwap(&other->texturename_);
  texturepath_.InternalSwap(&other->texturepath_);
  floatname_.InternalSwap(&other->floatname_);
  floatvalue_.InternalSwap(&other->floatvalue_);
  vec4name_.InternalSwap(&other->vec4name_);
  vec4value_.InternalSwap(&other->vec4value_);
  shadername_.Swap(&other->shadername_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  vshaderpath_.Swap(&other->vshaderpath_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  fshaderpath_.Swap(&other->fshaderpath_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  cubemapname_.Swap(&other->cubemapname_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(BBMaterial, dstblendfunc_)
      + sizeof(BBMaterial::dstblendfunc_)
      - PROTOBUF_FIELD_OFFSET(BBMaterial, cubemappaths_)>(
          reinterpret_cast<char*>(&cubemappaths_),
          reinterpret_cast<char*>(&other->cubemappaths_));
}

::PROTOBUF_NAMESPACE_ID::Metadata BBMaterial::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_BBMaterial_2eproto_getter, &descriptor_table_BBMaterial_2eproto_once,
      file_level_metadata_BBMaterial_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace BBSerializer
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::BBSerializer::BBMaterial* Arena::CreateMaybeMessage< ::BBSerializer::BBMaterial >(Arena* arena) {
  return Arena::CreateMessageInternal< ::BBSerializer::BBMaterial >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
