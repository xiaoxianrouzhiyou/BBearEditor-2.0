// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: BBCubeMap.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_BBCubeMap_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_BBCubeMap_2eproto

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
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_BBCubeMap_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_BBCubeMap_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_BBCubeMap_2eproto;
namespace BBSerializer {
class BBCubeMap;
struct BBCubeMapDefaultTypeInternal;
extern BBCubeMapDefaultTypeInternal _BBCubeMap_default_instance_;
}  // namespace BBSerializer
PROTOBUF_NAMESPACE_OPEN
template<> ::BBSerializer::BBCubeMap* Arena::CreateMaybeMessage<::BBSerializer::BBCubeMap>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace BBSerializer {

// ===================================================================

class BBCubeMap PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:BBSerializer.BBCubeMap) */ {
 public:
  inline BBCubeMap() : BBCubeMap(nullptr) {}
  ~BBCubeMap() override;
  explicit constexpr BBCubeMap(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  BBCubeMap(const BBCubeMap& from);
  BBCubeMap(BBCubeMap&& from) noexcept
    : BBCubeMap() {
    *this = ::std::move(from);
  }

  inline BBCubeMap& operator=(const BBCubeMap& from) {
    CopyFrom(from);
    return *this;
  }
  inline BBCubeMap& operator=(BBCubeMap&& from) noexcept {
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
  static const BBCubeMap& default_instance() {
    return *internal_default_instance();
  }
  static inline const BBCubeMap* internal_default_instance() {
    return reinterpret_cast<const BBCubeMap*>(
               &_BBCubeMap_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(BBCubeMap& a, BBCubeMap& b) {
    a.Swap(&b);
  }
  inline void Swap(BBCubeMap* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(BBCubeMap* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline BBCubeMap* New() const final {
    return CreateMaybeMessage<BBCubeMap>(nullptr);
  }

  BBCubeMap* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<BBCubeMap>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const BBCubeMap& from);
  void MergeFrom(const BBCubeMap& from);
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
  void InternalSwap(BBCubeMap* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "BBSerializer.BBCubeMap";
  }
  protected:
  explicit BBCubeMap(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kPositiveXFieldNumber = 1,
    kNegativeXFieldNumber = 2,
    kPositiveYFieldNumber = 3,
    kNegativeYFieldNumber = 4,
    kPositiveZFieldNumber = 5,
    kNegativeZFieldNumber = 6,
  };
  // string positiveX = 1;
  bool has_positivex() const;
  private:
  bool _internal_has_positivex() const;
  public:
  void clear_positivex();
  const std::string& positivex() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_positivex(ArgT0&& arg0, ArgT... args);
  std::string* mutable_positivex();
  std::string* release_positivex();
  void set_allocated_positivex(std::string* positivex);
  private:
  const std::string& _internal_positivex() const;
  void _internal_set_positivex(const std::string& value);
  std::string* _internal_mutable_positivex();
  public:

  // string negativeX = 2;
  bool has_negativex() const;
  private:
  bool _internal_has_negativex() const;
  public:
  void clear_negativex();
  const std::string& negativex() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_negativex(ArgT0&& arg0, ArgT... args);
  std::string* mutable_negativex();
  std::string* release_negativex();
  void set_allocated_negativex(std::string* negativex);
  private:
  const std::string& _internal_negativex() const;
  void _internal_set_negativex(const std::string& value);
  std::string* _internal_mutable_negativex();
  public:

  // string positiveY = 3;
  bool has_positivey() const;
  private:
  bool _internal_has_positivey() const;
  public:
  void clear_positivey();
  const std::string& positivey() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_positivey(ArgT0&& arg0, ArgT... args);
  std::string* mutable_positivey();
  std::string* release_positivey();
  void set_allocated_positivey(std::string* positivey);
  private:
  const std::string& _internal_positivey() const;
  void _internal_set_positivey(const std::string& value);
  std::string* _internal_mutable_positivey();
  public:

  // string negativeY = 4;
  bool has_negativey() const;
  private:
  bool _internal_has_negativey() const;
  public:
  void clear_negativey();
  const std::string& negativey() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_negativey(ArgT0&& arg0, ArgT... args);
  std::string* mutable_negativey();
  std::string* release_negativey();
  void set_allocated_negativey(std::string* negativey);
  private:
  const std::string& _internal_negativey() const;
  void _internal_set_negativey(const std::string& value);
  std::string* _internal_mutable_negativey();
  public:

  // string positiveZ = 5;
  bool has_positivez() const;
  private:
  bool _internal_has_positivez() const;
  public:
  void clear_positivez();
  const std::string& positivez() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_positivez(ArgT0&& arg0, ArgT... args);
  std::string* mutable_positivez();
  std::string* release_positivez();
  void set_allocated_positivez(std::string* positivez);
  private:
  const std::string& _internal_positivez() const;
  void _internal_set_positivez(const std::string& value);
  std::string* _internal_mutable_positivez();
  public:

  // string negativeZ = 6;
  bool has_negativez() const;
  private:
  bool _internal_has_negativez() const;
  public:
  void clear_negativez();
  const std::string& negativez() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_negativez(ArgT0&& arg0, ArgT... args);
  std::string* mutable_negativez();
  std::string* release_negativez();
  void set_allocated_negativez(std::string* negativez);
  private:
  const std::string& _internal_negativez() const;
  void _internal_set_negativez(const std::string& value);
  std::string* _internal_mutable_negativez();
  public:

  // @@protoc_insertion_point(class_scope:BBSerializer.BBCubeMap)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr positivex_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr negativex_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr positivey_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr negativey_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr positivez_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr negativez_;
  friend struct ::TableStruct_BBCubeMap_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// BBCubeMap

// string positiveX = 1;
inline bool BBCubeMap::_internal_has_positivex() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool BBCubeMap::has_positivex() const {
  return _internal_has_positivex();
}
inline void BBCubeMap::clear_positivex() {
  positivex_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& BBCubeMap::positivex() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBCubeMap.positiveX)
  return _internal_positivex();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBCubeMap::set_positivex(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000001u;
 positivex_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBCubeMap.positiveX)
}
inline std::string* BBCubeMap::mutable_positivex() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBCubeMap.positiveX)
  return _internal_mutable_positivex();
}
inline const std::string& BBCubeMap::_internal_positivex() const {
  return positivex_.Get();
}
inline void BBCubeMap::_internal_set_positivex(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  positivex_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBCubeMap::_internal_mutable_positivex() {
  _has_bits_[0] |= 0x00000001u;
  return positivex_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBCubeMap::release_positivex() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBCubeMap.positiveX)
  if (!_internal_has_positivex()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return positivex_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBCubeMap::set_allocated_positivex(std::string* positivex) {
  if (positivex != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  positivex_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), positivex,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBCubeMap.positiveX)
}

// string negativeX = 2;
inline bool BBCubeMap::_internal_has_negativex() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool BBCubeMap::has_negativex() const {
  return _internal_has_negativex();
}
inline void BBCubeMap::clear_negativex() {
  negativex_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000002u;
}
inline const std::string& BBCubeMap::negativex() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBCubeMap.negativeX)
  return _internal_negativex();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBCubeMap::set_negativex(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000002u;
 negativex_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBCubeMap.negativeX)
}
inline std::string* BBCubeMap::mutable_negativex() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBCubeMap.negativeX)
  return _internal_mutable_negativex();
}
inline const std::string& BBCubeMap::_internal_negativex() const {
  return negativex_.Get();
}
inline void BBCubeMap::_internal_set_negativex(const std::string& value) {
  _has_bits_[0] |= 0x00000002u;
  negativex_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBCubeMap::_internal_mutable_negativex() {
  _has_bits_[0] |= 0x00000002u;
  return negativex_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBCubeMap::release_negativex() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBCubeMap.negativeX)
  if (!_internal_has_negativex()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000002u;
  return negativex_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBCubeMap::set_allocated_negativex(std::string* negativex) {
  if (negativex != nullptr) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  negativex_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), negativex,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBCubeMap.negativeX)
}

// string positiveY = 3;
inline bool BBCubeMap::_internal_has_positivey() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool BBCubeMap::has_positivey() const {
  return _internal_has_positivey();
}
inline void BBCubeMap::clear_positivey() {
  positivey_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000004u;
}
inline const std::string& BBCubeMap::positivey() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBCubeMap.positiveY)
  return _internal_positivey();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBCubeMap::set_positivey(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000004u;
 positivey_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBCubeMap.positiveY)
}
inline std::string* BBCubeMap::mutable_positivey() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBCubeMap.positiveY)
  return _internal_mutable_positivey();
}
inline const std::string& BBCubeMap::_internal_positivey() const {
  return positivey_.Get();
}
inline void BBCubeMap::_internal_set_positivey(const std::string& value) {
  _has_bits_[0] |= 0x00000004u;
  positivey_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBCubeMap::_internal_mutable_positivey() {
  _has_bits_[0] |= 0x00000004u;
  return positivey_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBCubeMap::release_positivey() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBCubeMap.positiveY)
  if (!_internal_has_positivey()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000004u;
  return positivey_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBCubeMap::set_allocated_positivey(std::string* positivey) {
  if (positivey != nullptr) {
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  positivey_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), positivey,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBCubeMap.positiveY)
}

// string negativeY = 4;
inline bool BBCubeMap::_internal_has_negativey() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool BBCubeMap::has_negativey() const {
  return _internal_has_negativey();
}
inline void BBCubeMap::clear_negativey() {
  negativey_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000008u;
}
inline const std::string& BBCubeMap::negativey() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBCubeMap.negativeY)
  return _internal_negativey();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBCubeMap::set_negativey(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000008u;
 negativey_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBCubeMap.negativeY)
}
inline std::string* BBCubeMap::mutable_negativey() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBCubeMap.negativeY)
  return _internal_mutable_negativey();
}
inline const std::string& BBCubeMap::_internal_negativey() const {
  return negativey_.Get();
}
inline void BBCubeMap::_internal_set_negativey(const std::string& value) {
  _has_bits_[0] |= 0x00000008u;
  negativey_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBCubeMap::_internal_mutable_negativey() {
  _has_bits_[0] |= 0x00000008u;
  return negativey_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBCubeMap::release_negativey() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBCubeMap.negativeY)
  if (!_internal_has_negativey()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000008u;
  return negativey_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBCubeMap::set_allocated_negativey(std::string* negativey) {
  if (negativey != nullptr) {
    _has_bits_[0] |= 0x00000008u;
  } else {
    _has_bits_[0] &= ~0x00000008u;
  }
  negativey_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), negativey,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBCubeMap.negativeY)
}

// string positiveZ = 5;
inline bool BBCubeMap::_internal_has_positivez() const {
  bool value = (_has_bits_[0] & 0x00000010u) != 0;
  return value;
}
inline bool BBCubeMap::has_positivez() const {
  return _internal_has_positivez();
}
inline void BBCubeMap::clear_positivez() {
  positivez_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000010u;
}
inline const std::string& BBCubeMap::positivez() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBCubeMap.positiveZ)
  return _internal_positivez();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBCubeMap::set_positivez(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000010u;
 positivez_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBCubeMap.positiveZ)
}
inline std::string* BBCubeMap::mutable_positivez() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBCubeMap.positiveZ)
  return _internal_mutable_positivez();
}
inline const std::string& BBCubeMap::_internal_positivez() const {
  return positivez_.Get();
}
inline void BBCubeMap::_internal_set_positivez(const std::string& value) {
  _has_bits_[0] |= 0x00000010u;
  positivez_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBCubeMap::_internal_mutable_positivez() {
  _has_bits_[0] |= 0x00000010u;
  return positivez_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBCubeMap::release_positivez() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBCubeMap.positiveZ)
  if (!_internal_has_positivez()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000010u;
  return positivez_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBCubeMap::set_allocated_positivez(std::string* positivez) {
  if (positivez != nullptr) {
    _has_bits_[0] |= 0x00000010u;
  } else {
    _has_bits_[0] &= ~0x00000010u;
  }
  positivez_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), positivez,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBCubeMap.positiveZ)
}

// string negativeZ = 6;
inline bool BBCubeMap::_internal_has_negativez() const {
  bool value = (_has_bits_[0] & 0x00000020u) != 0;
  return value;
}
inline bool BBCubeMap::has_negativez() const {
  return _internal_has_negativez();
}
inline void BBCubeMap::clear_negativez() {
  negativez_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000020u;
}
inline const std::string& BBCubeMap::negativez() const {
  // @@protoc_insertion_point(field_get:BBSerializer.BBCubeMap.negativeZ)
  return _internal_negativez();
}
template <typename ArgT0, typename... ArgT>
PROTOBUF_ALWAYS_INLINE
inline void BBCubeMap::set_negativez(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000020u;
 negativez_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArena());
  // @@protoc_insertion_point(field_set:BBSerializer.BBCubeMap.negativeZ)
}
inline std::string* BBCubeMap::mutable_negativez() {
  // @@protoc_insertion_point(field_mutable:BBSerializer.BBCubeMap.negativeZ)
  return _internal_mutable_negativez();
}
inline const std::string& BBCubeMap::_internal_negativez() const {
  return negativez_.Get();
}
inline void BBCubeMap::_internal_set_negativez(const std::string& value) {
  _has_bits_[0] |= 0x00000020u;
  negativez_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline std::string* BBCubeMap::_internal_mutable_negativez() {
  _has_bits_[0] |= 0x00000020u;
  return negativez_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* BBCubeMap::release_negativez() {
  // @@protoc_insertion_point(field_release:BBSerializer.BBCubeMap.negativeZ)
  if (!_internal_has_negativez()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000020u;
  return negativez_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void BBCubeMap::set_allocated_negativez(std::string* negativez) {
  if (negativez != nullptr) {
    _has_bits_[0] |= 0x00000020u;
  } else {
    _has_bits_[0] &= ~0x00000020u;
  }
  negativez_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), negativez,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:BBSerializer.BBCubeMap.negativeZ)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace BBSerializer

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_BBCubeMap_2eproto
