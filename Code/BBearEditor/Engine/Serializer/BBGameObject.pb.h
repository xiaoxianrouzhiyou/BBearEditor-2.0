// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BBGameObject.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_BBGameObject_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_BBGameObject_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3016000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3016000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "BBVector.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_BBGameObject_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_BBGameObject_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_BBGameObject_2eproto;
namespace BBSerializer {
class BBGameObject;
struct BBGameObjectDefaultTypeInternal;
extern BBGameObjectDefaultTypeInternal _BBGameObject_default_instance_;
}  // namespace BBSerializer
PROTOBUF_NAMESPACE_OPEN
template<> ::BBSerializer::BBGameObject* Arena::CreateMaybeMessage<::BBSerializer::BBGameObject>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace BBSerializer {

// ===================================================================

class BBGameObject PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:BBSerializer.BBGameObject) */ {
 public:
  inline BBGameObject() : BBGameObject(nullptr) {}
  ~BBGameObject() override;
  explicit constexpr BBGameObject(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  BBGameObject(const BBGameObject& from);
  BBGameObject(BBGameObject&& from) noexcept
    : BBGameObject() {
    *this = ::std::move(from);
  }

  inline BBGameObject& operator=(const BBGameObject& from) {
    CopyFrom(from);
    return *this;
  }
  inline BBGameObject& operator=(BBGameObject&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const BBGameObject& default_instance() {
    return *internal_default_instance();
  }
  static inline const BBGameObject* internal_default_instance() {
    return reinterpret_cast<const BBGameObject*>(
               &_BBGameObject_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(BBGameObject& a, BBGameObject& b) {
    a.Swap(&b);
  }
  inline void Swap(BBGameObject* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(BBGameObject* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline BBGameObject* New() const final {
    return CreateMaybeMessage<BBGameObject>(nullptr);
  }

  BBGameObject* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<BBGameObject>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const BBGameObject& from);
  void MergeFrom(const BBGameObject& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(BBGameObject* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "BBSerializer.BBGameObject";
  }
  protected:
  explicit BBGameObject(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
    kClassNameFieldNumber = 2,
    kFilePathFieldNumber = 3,
    kPositionFieldNumber = 4,
    kLocalPositionFieldNumber = 5,
    kRotationFieldNumber = 6,
    kLocalRotationFieldNumber = 7,
    kScaleFieldNumber = 8,
    kLocalScaleFieldNumber = 9,
  };
  // string name = 1;
  bool has_name() const;
  private:
  bool _internal_has_name() const;
  public:
  void clear_name();
  const std::string& name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // string className = 2;
  bool has_classname() const;
  private:
  bool _internal_has_classname() const;
  public:
  void clear_classname();
  const std::string& classname() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_classname(ArgT0&& arg0, ArgT... args);
  std::string* mutable_classname();
  std::string* release_classname();
  void set_allocated_classname(std::string* classname);
  private:
  const std::string& _internal_classname() const;
  void _internal_set_classname(const std::string& value);
  std::string* _internal_mutable_classname();
  public:

  // string filePath = 3;
  bool has_filepath() const;
  private:
  bool _internal_has_filepath() const;
  public:
  void clear_filepath();
  const std::string& filepath() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_filepath(ArgT0&& arg0, ArgT... args);
  std::string* mutable_filepath();
  std::string* release_filepath();
  void set_allocated_filepath(std::string* filepath);
  private:
  const std::string& _internal_filepath() const;
  void _internal_set_filepath(const std::string& value);
  std::string* _internal_mutable_filepath();
  public:

  // .BBSerializer.BBVector3f position = 4;
  bool has_position() const;
  private:
  bool _internal_has_position() const;
  public:
  void clear_position();
  const ::BBSerializer::BBVector3f& position() const;
  ::BBSerializer::BBVector3f* release_position();
  ::BBSerializer::BBVector3f* mutable_position();
  void set_allocated_position(::BBSerializer::BBVector3f* position);
  private:
  const ::BBSerializer::BBVector3f& _internal_position() const;
  ::BBSerializer::BBVector3f* _internal_mutable_position();
  public:
  void unsafe_arena_set_allocated_position(
      ::BBSerializer::BBVector3f* position);
  ::BBSerializer::BBVector3f* unsafe_arena_release_position();

  // .BBSerializer.BBVector3f localPosition = 5;
  bool has_localposition() const;
  private:
  bool _internal_has_localposition() const;
  public:
  void clear_localposition();
  const ::BBSerializer::BBVector3f& localposition() const;
  ::BBSerializer::BBVector3f* release_localposition();
  ::BBSerializer::BBVector3f* mutable_localposition();
  void set_allocated_localposition(::BBSerializer::BBVector3f* localposition);
  private:
  const ::BBSerializer::BBVector3f& _internal_localposition() const;
  ::BBSerializer::BBVector3f* _internal_mutable_localposition();
  public:
  void unsafe_arena_set_allocated_localposition(
      ::BBSerializer::BBVector3f* localposition);
  ::BBSerializer::BBVector3f* unsafe_arena_release_localposition();

  // .BBSerializer.BBVector3f rotation = 6;
  bool has_rotation() const;
  private:
  bool _internal_has_rotation() const;
  public:
  void clear_rotation();
  const ::BBSerializer::BBVector3f& rotation() const;
  ::BBSerializer::BBVector3f* release_rotation();
  ::BBSerializer::BBVector3f* mutable_rotation();
  void set_allocated_rotation(::BBSerializer::BBVector3f* rotation);
  private:
  const ::BBSerializer::BBVector3f& _internal_rotation() const;
  ::BBSerializer::BBVector3f* _internal_mutable_rotation();
  public:
  void unsafe_arena_set_allocated_rotation(
      ::BBSerializer::BBVector3f* rotation);
  ::BBSerializer::BBVector3f* unsafe_arena_release_rotation();

  // .BBSerializer.BBVector3f localRotation = 7;
  bool has_localrotation() const;
  private:
  bool _internal_has_localrotation() const;
  public:
  void clear_localrotation();
  const ::BBSerializer::BBVector3f& localrotation() const;
  ::BBSerializer::BBVector3f* release_localrotation();
  ::BBSerializer::BBVector3f* mutable_localrotation();
  void set_allocated_localrotation(::BBSerializer::BBVector3f* localrotation);
  private:
  const ::BBSerializer::BBVector3f& _internal_localrotation() const;
  ::BBSerializer::BBVector3f* _internal_mutable_localrotation();
  public:
  void unsafe_arena_set_allocated_localrotation(
      ::BBSerializer::BBVector3f* localrotation);
  ::BBSerializer::BBVector3f* unsafe_arena_release_localrotation();

  // .BBSerializer.BBVector3f scale = 8;
  bool has_scale() const;
  private:
  bool _internal_has_scale() const;
  public:
  void clear_scale();
  const ::BBSerializer::BBVector3f& scale() const;
  ::BBSerializer::BBVector3f* release_scale();
  ::BBSerializer::BBVector3f* mutable_scale();
  void set_allocated_scale(::BBSerializer::BBVector3f* scale);
  private:
  const ::BBSerializer::BBVector3f& _internal_scale() const;
  ::BBSerializer::BBVector3f* _internal_mutable_scale();
  public:
  void unsafe_arena_set_allocated_scale(
      ::BBSerializer::BBVector3f* scale);
  ::BBSerializer::BBVector3f* unsafe_arena_release_scale();

  // .BBSerializer.BBVector3f localScale = 9;
  bool has_localscale() const;
  private:
  bool _internal_has_localscale() const;
  public:
  void clear_localscale();
  const ::BBSerializer::BBVector3f& localscale() const;
  ::BBSerializer::BBVector3f* release_localscale();
  ::BBSerializer::BBVector3f* mutable_localscale();
  void set_allocated_localscale(::BBSerializer::BBVector3f* localscale);
  private:
  const ::BBSerializer::BBVector3f& _internal_localscale() const;
  ::BBSerializer::BBVector3f* _internal_mutable_localscale();
  public:
  void unsafe_arena_set_allocated_localscale(
      ::BBSerializer::BBVector3f* localscale);
  ::BBSerializer::BBVector3f* unsafe_arena_release_localscale();

  // @@protoc_insertion_point(class_scope:BBSerializer.BBGameObject)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr classname_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr filepath_;
  ::BBSerializer::BBVector3f* position_;
  ::BBSerializer::BBVector3f* localposition_;
  ::BBSerializer::BBVector3f* rotation_;
  ::BBSerializer::BBVector3f* localrotation_;
  ::BBSerializer::BBVector3f* scale_;
  ::BBSerializer::BBVector3f* localscale_;
  friend struct ::TableStruct_BBGameObject_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// BBGameObject

// string name = 1;
inline bool BBGameObject::_internal_has_name() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool BBGameObject::has_name() const {
  return _internal_has_name();
}
inline void BBGameObject::clear_name() {
  name_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& BBGameObject::name() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.name)
  return _internal_name();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBGameObject::set_name(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000001u;
 name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBGameObject.name)
}
inline std::string* BBGameObject::mutable_name() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.name)
  return _internal_mutable_name();
}
inline const std::string& BBGameObject::_internal_name() const {
  return name_.Get();
}
inline void BBGameObject::_internal_set_name(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBGameObject::_internal_mutable_name() {
  _has_bits_[0] |= 0x00000001u;
  return name_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBGameObject::release_name() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.name)
  if (!_internal_has_name()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return name_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBGameObject::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.name)
}

// string className = 2;
inline bool BBGameObject::_internal_has_classname() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool BBGameObject::has_classname() const {
  return _internal_has_classname();
}
inline void BBGameObject::clear_classname() {
  classname_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000002u;
}
inline const std::string& BBGameObject::classname() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.className)
  return _internal_classname();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBGameObject::set_classname(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000002u;
 classname_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBGameObject.className)
}
inline std::string* BBGameObject::mutable_classname() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.className)
  return _internal_mutable_classname();
}
inline const std::string& BBGameObject::_internal_classname() const {
  return classname_.Get();
}
inline void BBGameObject::_internal_set_classname(const std::string& value) {
  _has_bits_[0] |= 0x00000002u;
  classname_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBGameObject::_internal_mutable_classname() {
  _has_bits_[0] |= 0x00000002u;
  return classname_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBGameObject::release_classname() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.className)
  if (!_internal_has_classname()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000002u;
  return classname_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBGameObject::set_allocated_classname(std::string* classname) {
  if (classname != nullptr) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  classname_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), classname,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.className)
}

// string filePath = 3;
inline bool BBGameObject::_internal_has_filepath() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool BBGameObject::has_filepath() const {
  return _internal_has_filepath();
}
inline void BBGameObject::clear_filepath() {
  filepath_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000004u;
}
inline const std::string& BBGameObject::filepath() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.filePath)
  return _internal_filepath();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBGameObject::set_filepath(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000004u;
 filepath_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBGameObject.filePath)
}
inline std::string* BBGameObject::mutable_filepath() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.filePath)
  return _internal_mutable_filepath();
}
inline const std::string& BBGameObject::_internal_filepath() const {
  return filepath_.Get();
}
inline void BBGameObject::_internal_set_filepath(const std::string& value) {
  _has_bits_[0] |= 0x00000004u;
  filepath_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBGameObject::_internal_mutable_filepath() {
  _has_bits_[0] |= 0x00000004u;
  return filepath_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBGameObject::release_filepath() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.filePath)
  if (!_internal_has_filepath()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000004u;
  return filepath_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBGameObject::set_allocated_filepath(std::string* filepath) {
  if (filepath != nullptr) {
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  filepath_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), filepath,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.filePath)
}

// .BBSerializer.BBVector3f position = 4;
inline bool BBGameObject::_internal_has_position() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  PROTOBUF_ASSUME(!value || position_ != nullptr);
  return value;
}
inline bool BBGameObject::has_position() const {
  return _internal_has_position();
}
inline const ::BBSerializer::BBVector3f& BBGameObject::_internal_position() const {
  const ::BBSerializer::BBVector3f* p = position_;
  return p != nullptr ? *p : reinterpret_cast<const ::BBSerializer::BBVector3f&>(
      ::BBSerializer::_BBVector3f_default_instance_);
}
inline const ::BBSerializer::BBVector3f& BBGameObject::position() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.position)
  return _internal_position();
}
inline void BBGameObject::unsafe_arena_set_allocated_position(
    ::BBSerializer::BBVector3f* position) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(position_);
  }
  position_ = position;
  if (position) {
    _has_bits_[0] |= 0x00000008u;
  } else {
    _has_bits_[0] &= ~0x00000008u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:BBSerializer.BBGameObject.position)
}
inline ::BBSerializer::BBVector3f* BBGameObject::release_position() {
  _has_bits_[0] &= ~0x00000008u;
  ::BBSerializer::BBVector3f* temp = position_;
  position_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::unsafe_arena_release_position() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.position)
  _has_bits_[0] &= ~0x00000008u;
  ::BBSerializer::BBVector3f* temp = position_;
  position_ = nullptr;
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::_internal_mutable_position() {
  _has_bits_[0] |= 0x00000008u;
  if (position_ == nullptr) {
    auto* p = CreateMaybeMessage<::BBSerializer::BBVector3f>(GetArena());
    position_ = p;
  }
  return position_;
}
inline ::BBSerializer::BBVector3f* BBGameObject::mutable_position() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.position)
  return _internal_mutable_position();
}
inline void BBGameObject::set_allocated_position(::BBSerializer::BBVector3f* position) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(position_);
  }
  if (position) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(position)->GetArena();
    if (message_arena != submessage_arena) {
      position = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, position, submessage_arena);
    }
    _has_bits_[0] |= 0x00000008u;
  } else {
    _has_bits_[0] &= ~0x00000008u;
  }
  position_ = position;
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.position)
}

// .BBSerializer.BBVector3f localPosition = 5;
inline bool BBGameObject::_internal_has_localposition() const {
  bool value = (_has_bits_[0] & 0x00000010u) != 0;
  PROTOBUF_ASSUME(!value || localposition_ != nullptr);
  return value;
}
inline bool BBGameObject::has_localposition() const {
  return _internal_has_localposition();
}
inline const ::BBSerializer::BBVector3f& BBGameObject::_internal_localposition() const {
  const ::BBSerializer::BBVector3f* p = localposition_;
  return p != nullptr ? *p : reinterpret_cast<const ::BBSerializer::BBVector3f&>(
      ::BBSerializer::_BBVector3f_default_instance_);
}
inline const ::BBSerializer::BBVector3f& BBGameObject::localposition() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.localPosition)
  return _internal_localposition();
}
inline void BBGameObject::unsafe_arena_set_allocated_localposition(
    ::BBSerializer::BBVector3f* localposition) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(localposition_);
  }
  localposition_ = localposition;
  if (localposition) {
    _has_bits_[0] |= 0x00000010u;
  } else {
    _has_bits_[0] &= ~0x00000010u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:BBSerializer.BBGameObject.localPosition)
}
inline ::BBSerializer::BBVector3f* BBGameObject::release_localposition() {
  _has_bits_[0] &= ~0x00000010u;
  ::BBSerializer::BBVector3f* temp = localposition_;
  localposition_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::unsafe_arena_release_localposition() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.localPosition)
  _has_bits_[0] &= ~0x00000010u;
  ::BBSerializer::BBVector3f* temp = localposition_;
  localposition_ = nullptr;
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::_internal_mutable_localposition() {
  _has_bits_[0] |= 0x00000010u;
  if (localposition_ == nullptr) {
    auto* p = CreateMaybeMessage<::BBSerializer::BBVector3f>(GetArena());
    localposition_ = p;
  }
  return localposition_;
}
inline ::BBSerializer::BBVector3f* BBGameObject::mutable_localposition() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.localPosition)
  return _internal_mutable_localposition();
}
inline void BBGameObject::set_allocated_localposition(::BBSerializer::BBVector3f* localposition) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(localposition_);
  }
  if (localposition) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(localposition)->GetArena();
    if (message_arena != submessage_arena) {
      localposition = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, localposition, submessage_arena);
    }
    _has_bits_[0] |= 0x00000010u;
  } else {
    _has_bits_[0] &= ~0x00000010u;
  }
  localposition_ = localposition;
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.localPosition)
}

// .BBSerializer.BBVector3f rotation = 6;
inline bool BBGameObject::_internal_has_rotation() const {
  bool value = (_has_bits_[0] & 0x00000020u) != 0;
  PROTOBUF_ASSUME(!value || rotation_ != nullptr);
  return value;
}
inline bool BBGameObject::has_rotation() const {
  return _internal_has_rotation();
}
inline const ::BBSerializer::BBVector3f& BBGameObject::_internal_rotation() const {
  const ::BBSerializer::BBVector3f* p = rotation_;
  return p != nullptr ? *p : reinterpret_cast<const ::BBSerializer::BBVector3f&>(
      ::BBSerializer::_BBVector3f_default_instance_);
}
inline const ::BBSerializer::BBVector3f& BBGameObject::rotation() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.rotation)
  return _internal_rotation();
}
inline void BBGameObject::unsafe_arena_set_allocated_rotation(
    ::BBSerializer::BBVector3f* rotation) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(rotation_);
  }
  rotation_ = rotation;
  if (rotation) {
    _has_bits_[0] |= 0x00000020u;
  } else {
    _has_bits_[0] &= ~0x00000020u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:BBSerializer.BBGameObject.rotation)
}
inline ::BBSerializer::BBVector3f* BBGameObject::release_rotation() {
  _has_bits_[0] &= ~0x00000020u;
  ::BBSerializer::BBVector3f* temp = rotation_;
  rotation_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::unsafe_arena_release_rotation() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.rotation)
  _has_bits_[0] &= ~0x00000020u;
  ::BBSerializer::BBVector3f* temp = rotation_;
  rotation_ = nullptr;
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::_internal_mutable_rotation() {
  _has_bits_[0] |= 0x00000020u;
  if (rotation_ == nullptr) {
    auto* p = CreateMaybeMessage<::BBSerializer::BBVector3f>(GetArena());
    rotation_ = p;
  }
  return rotation_;
}
inline ::BBSerializer::BBVector3f* BBGameObject::mutable_rotation() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.rotation)
  return _internal_mutable_rotation();
}
inline void BBGameObject::set_allocated_rotation(::BBSerializer::BBVector3f* rotation) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(rotation_);
  }
  if (rotation) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(rotation)->GetArena();
    if (message_arena != submessage_arena) {
      rotation = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, rotation, submessage_arena);
    }
    _has_bits_[0] |= 0x00000020u;
  } else {
    _has_bits_[0] &= ~0x00000020u;
  }
  rotation_ = rotation;
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.rotation)
}

// .BBSerializer.BBVector3f localRotation = 7;
inline bool BBGameObject::_internal_has_localrotation() const {
  bool value = (_has_bits_[0] & 0x00000040u) != 0;
  PROTOBUF_ASSUME(!value || localrotation_ != nullptr);
  return value;
}
inline bool BBGameObject::has_localrotation() const {
  return _internal_has_localrotation();
}
inline const ::BBSerializer::BBVector3f& BBGameObject::_internal_localrotation() const {
  const ::BBSerializer::BBVector3f* p = localrotation_;
  return p != nullptr ? *p : reinterpret_cast<const ::BBSerializer::BBVector3f&>(
      ::BBSerializer::_BBVector3f_default_instance_);
}
inline const ::BBSerializer::BBVector3f& BBGameObject::localrotation() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.localRotation)
  return _internal_localrotation();
}
inline void BBGameObject::unsafe_arena_set_allocated_localrotation(
    ::BBSerializer::BBVector3f* localrotation) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(localrotation_);
  }
  localrotation_ = localrotation;
  if (localrotation) {
    _has_bits_[0] |= 0x00000040u;
  } else {
    _has_bits_[0] &= ~0x00000040u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:BBSerializer.BBGameObject.localRotation)
}
inline ::BBSerializer::BBVector3f* BBGameObject::release_localrotation() {
  _has_bits_[0] &= ~0x00000040u;
  ::BBSerializer::BBVector3f* temp = localrotation_;
  localrotation_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::unsafe_arena_release_localrotation() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.localRotation)
  _has_bits_[0] &= ~0x00000040u;
  ::BBSerializer::BBVector3f* temp = localrotation_;
  localrotation_ = nullptr;
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::_internal_mutable_localrotation() {
  _has_bits_[0] |= 0x00000040u;
  if (localrotation_ == nullptr) {
    auto* p = CreateMaybeMessage<::BBSerializer::BBVector3f>(GetArena());
    localrotation_ = p;
  }
  return localrotation_;
}
inline ::BBSerializer::BBVector3f* BBGameObject::mutable_localrotation() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.localRotation)
  return _internal_mutable_localrotation();
}
inline void BBGameObject::set_allocated_localrotation(::BBSerializer::BBVector3f* localrotation) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(localrotation_);
  }
  if (localrotation) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(localrotation)->GetArena();
    if (message_arena != submessage_arena) {
      localrotation = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, localrotation, submessage_arena);
    }
    _has_bits_[0] |= 0x00000040u;
  } else {
    _has_bits_[0] &= ~0x00000040u;
  }
  localrotation_ = localrotation;
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.localRotation)
}

// .BBSerializer.BBVector3f scale = 8;
inline bool BBGameObject::_internal_has_scale() const {
  bool value = (_has_bits_[0] & 0x00000080u) != 0;
  PROTOBUF_ASSUME(!value || scale_ != nullptr);
  return value;
}
inline bool BBGameObject::has_scale() const {
  return _internal_has_scale();
}
inline const ::BBSerializer::BBVector3f& BBGameObject::_internal_scale() const {
  const ::BBSerializer::BBVector3f* p = scale_;
  return p != nullptr ? *p : reinterpret_cast<const ::BBSerializer::BBVector3f&>(
      ::BBSerializer::_BBVector3f_default_instance_);
}
inline const ::BBSerializer::BBVector3f& BBGameObject::scale() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.scale)
  return _internal_scale();
}
inline void BBGameObject::unsafe_arena_set_allocated_scale(
    ::BBSerializer::BBVector3f* scale) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(scale_);
  }
  scale_ = scale;
  if (scale) {
    _has_bits_[0] |= 0x00000080u;
  } else {
    _has_bits_[0] &= ~0x00000080u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:BBSerializer.BBGameObject.scale)
}
inline ::BBSerializer::BBVector3f* BBGameObject::release_scale() {
  _has_bits_[0] &= ~0x00000080u;
  ::BBSerializer::BBVector3f* temp = scale_;
  scale_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::unsafe_arena_release_scale() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.scale)
  _has_bits_[0] &= ~0x00000080u;
  ::BBSerializer::BBVector3f* temp = scale_;
  scale_ = nullptr;
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::_internal_mutable_scale() {
  _has_bits_[0] |= 0x00000080u;
  if (scale_ == nullptr) {
    auto* p = CreateMaybeMessage<::BBSerializer::BBVector3f>(GetArena());
    scale_ = p;
  }
  return scale_;
}
inline ::BBSerializer::BBVector3f* BBGameObject::mutable_scale() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.scale)
  return _internal_mutable_scale();
}
inline void BBGameObject::set_allocated_scale(::BBSerializer::BBVector3f* scale) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(scale_);
  }
  if (scale) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(scale)->GetArena();
    if (message_arena != submessage_arena) {
      scale = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, scale, submessage_arena);
    }
    _has_bits_[0] |= 0x00000080u;
  } else {
    _has_bits_[0] &= ~0x00000080u;
  }
  scale_ = scale;
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.scale)
}

// .BBSerializer.BBVector3f localScale = 9;
inline bool BBGameObject::_internal_has_localscale() const {
  bool value = (_has_bits_[0] & 0x00000100u) != 0;
  PROTOBUF_ASSUME(!value || localscale_ != nullptr);
  return value;
}
inline bool BBGameObject::has_localscale() const {
  return _internal_has_localscale();
}
inline const ::BBSerializer::BBVector3f& BBGameObject::_internal_localscale() const {
  const ::BBSerializer::BBVector3f* p = localscale_;
  return p != nullptr ? *p : reinterpret_cast<const ::BBSerializer::BBVector3f&>(
      ::BBSerializer::_BBVector3f_default_instance_);
}
inline const ::BBSerializer::BBVector3f& BBGameObject::localscale() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBGameObject.localScale)
  return _internal_localscale();
}
inline void BBGameObject::unsafe_arena_set_allocated_localscale(
    ::BBSerializer::BBVector3f* localscale) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(localscale_);
  }
  localscale_ = localscale;
  if (localscale) {
    _has_bits_[0] |= 0x00000100u;
  } else {
    _has_bits_[0] &= ~0x00000100u;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:BBSerializer.BBGameObject.localScale)
}
inline ::BBSerializer::BBVector3f* BBGameObject::release_localscale() {
  _has_bits_[0] &= ~0x00000100u;
  ::BBSerializer::BBVector3f* temp = localscale_;
  localscale_ = nullptr;
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::unsafe_arena_release_localscale() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBGameObject.localScale)
  _has_bits_[0] &= ~0x00000100u;
  ::BBSerializer::BBVector3f* temp = localscale_;
  localscale_ = nullptr;
  return temp;
}
inline ::BBSerializer::BBVector3f* BBGameObject::_internal_mutable_localscale() {
  _has_bits_[0] |= 0x00000100u;
  if (localscale_ == nullptr) {
    auto* p = CreateMaybeMessage<::BBSerializer::BBVector3f>(GetArena());
    localscale_ = p;
  }
  return localscale_;
}
inline ::BBSerializer::BBVector3f* BBGameObject::mutable_localscale() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBGameObject.localScale)
  return _internal_mutable_localscale();
}
inline void BBGameObject::set_allocated_localscale(::BBSerializer::BBVector3f* localscale) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(localscale_);
  }
  if (localscale) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(localscale)->GetArena();
    if (message_arena != submessage_arena) {
      localscale = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, localscale, submessage_arena);
    }
    _has_bits_[0] |= 0x00000100u;
  } else {
    _has_bits_[0] &= ~0x00000100u;
  }
  localscale_ = localscale;
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBGameObject.localScale)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace BBSerializer

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_BBGameObject_2eproto
