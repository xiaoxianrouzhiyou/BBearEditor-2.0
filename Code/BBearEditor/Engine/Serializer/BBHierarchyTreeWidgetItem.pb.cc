// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BBHierarchyTreeWidgetItem.proto

#include "BBHierarchyTreeWidgetItem.pb.h"

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
constexpr BBHierarchyTreeWidgetItem::BBHierarchyTreeWidgetItem(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : parentindex_(0){}
struct BBHierarchyTreeWidgetItemDefaultTypeInternal {
  constexpr BBHierarchyTreeWidgetItemDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~BBHierarchyTreeWidgetItemDefaultTypeInternal() {}
  union {
    BBHierarchyTreeWidgetItem _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT BBHierarchyTreeWidgetItemDefaultTypeInternal _BBHierarchyTreeWidgetItem_default_instance_;
}  // namespace BBSerializer
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_BBHierarchyTreeWidgetItem_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_BBHierarchyTreeWidgetItem_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_BBHierarchyTreeWidgetItem_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_BBHierarchyTreeWidgetItem_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBHierarchyTreeWidgetItem, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBHierarchyTreeWidgetItem, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::BBSerializer::BBHierarchyTreeWidgetItem, parentindex_),
  0,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 6, sizeof(::BBSerializer::BBHierarchyTreeWidgetItem)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::BBSerializer::_BBHierarchyTreeWidgetItem_default_instance_),
};

const char descriptor_table_protodef_BBHierarchyTreeWidgetItem_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\037BBHierarchyTreeWidgetItem.proto\022\014BBSer"
  "ializer\"E\n\031BBHierarchyTreeWidgetItem\022\030\n\013"
  "parentIndex\030\001 \001(\005H\000\210\001\001B\016\n\014_parentIndexb\006"
  "proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_BBHierarchyTreeWidgetItem_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_BBHierarchyTreeWidgetItem_2eproto = {
  false, false, 126, descriptor_table_protodef_BBHierarchyTreeWidgetItem_2eproto, "BBHierarchyTreeWidgetItem.proto", 
  &descriptor_table_BBHierarchyTreeWidgetItem_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_BBHierarchyTreeWidgetItem_2eproto::offsets,
  file_level_metadata_BBHierarchyTreeWidgetItem_2eproto, file_level_enum_descriptors_BBHierarchyTreeWidgetItem_2eproto, file_level_service_descriptors_BBHierarchyTreeWidgetItem_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_BBHierarchyTreeWidgetItem_2eproto_getter() {
  return &descriptor_table_BBHierarchyTreeWidgetItem_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_BBHierarchyTreeWidgetItem_2eproto(&descriptor_table_BBHierarchyTreeWidgetItem_2eproto);
namespace BBSerializer {

// ===================================================================

class BBHierarchyTreeWidgetItem::_Internal {
 public:
  using HasBits = decltype(std::declval<BBHierarchyTreeWidgetItem>()._has_bits_);
  static void set_has_parentindex(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

BBHierarchyTreeWidgetItem::BBHierarchyTreeWidgetItem(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:BBSerializer.BBHierarchyTreeWidgetItem)
}
BBHierarchyTreeWidgetItem::BBHierarchyTreeWidgetItem(const BBHierarchyTreeWidgetItem& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  parentindex_ = from.parentindex_;
  // @@protoc_insertion_point(copy_constructor:BBSerializer.BBHierarchyTreeWidgetItem)
}

void BBHierarchyTreeWidgetItem::SharedCtor() {
parentindex_ = 0;
}

BBHierarchyTreeWidgetItem::~BBHierarchyTreeWidgetItem() {
  // @@protoc_insertion_point(destructor:BBSerializer.BBHierarchyTreeWidgetItem)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void BBHierarchyTreeWidgetItem::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void BBHierarchyTreeWidgetItem::ArenaDtor(void* object) {
  BBHierarchyTreeWidgetItem* _this = reinterpret_cast< BBHierarchyTreeWidgetItem* >(object);
  (void)_this;
}
void BBHierarchyTreeWidgetItem::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void BBHierarchyTreeWidgetItem::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void BBHierarchyTreeWidgetItem::Clear() {
// @@protoc_insertion_point(message_clear_start:BBSerializer.BBHierarchyTreeWidgetItem)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  parentindex_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* BBHierarchyTreeWidgetItem::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 parentIndex = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_parentindex(&has_bits);
          parentindex_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* BBHierarchyTreeWidgetItem::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:BBSerializer.BBHierarchyTreeWidgetItem)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 parentIndex = 1;
  if (_internal_has_parentindex()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_parentindex(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:BBSerializer.BBHierarchyTreeWidgetItem)
  return target;
}

size_t BBHierarchyTreeWidgetItem::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:BBSerializer.BBHierarchyTreeWidgetItem)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 parentIndex = 1;
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_parentindex());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void BBHierarchyTreeWidgetItem::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:BBSerializer.BBHierarchyTreeWidgetItem)
  GOOGLE_DCHECK_NE(&from, this);
  const BBHierarchyTreeWidgetItem* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<BBHierarchyTreeWidgetItem>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:BBSerializer.BBHierarchyTreeWidgetItem)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:BBSerializer.BBHierarchyTreeWidgetItem)
    MergeFrom(*source);
  }
}

void BBHierarchyTreeWidgetItem::MergeFrom(const BBHierarchyTreeWidgetItem& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:BBSerializer.BBHierarchyTreeWidgetItem)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_parentindex()) {
    _internal_set_parentindex(from._internal_parentindex());
  }
}

void BBHierarchyTreeWidgetItem::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:BBSerializer.BBHierarchyTreeWidgetItem)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BBHierarchyTreeWidgetItem::CopyFrom(const BBHierarchyTreeWidgetItem& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:BBSerializer.BBHierarchyTreeWidgetItem)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BBHierarchyTreeWidgetItem::IsInitialized() const {
  return true;
}

void BBHierarchyTreeWidgetItem::InternalSwap(BBHierarchyTreeWidgetItem* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  swap(parentindex_, other->parentindex_);
}

::PROTOBUF_NAMESPACE_ID::Metadata BBHierarchyTreeWidgetItem::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_BBHierarchyTreeWidgetItem_2eproto_getter, &descriptor_table_BBHierarchyTreeWidgetItem_2eproto_once,
      file_level_metadata_BBHierarchyTreeWidgetItem_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace BBSerializer
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::BBSerializer::BBHierarchyTreeWidgetItem* Arena::CreateMaybeMessage< ::BBSerializer::BBHierarchyTreeWidgetItem >(Arena* arena) {
  return Arena::CreateMessageInternal< ::BBSerializer::BBHierarchyTreeWidgetItem >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
