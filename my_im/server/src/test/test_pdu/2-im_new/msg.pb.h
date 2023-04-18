// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_msg_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_msg_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012004 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_msg_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_msg_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_msg_2eproto;
namespace Im {
class helloworld;
class helloworldDefaultTypeInternal;
extern helloworldDefaultTypeInternal _helloworld_default_instance_;
class worldhello;
class worldhelloDefaultTypeInternal;
extern worldhelloDefaultTypeInternal _worldhello_default_instance_;
}  // namespace Im
PROTOBUF_NAMESPACE_OPEN
template<> ::Im::helloworld* Arena::CreateMaybeMessage<::Im::helloworld>(Arena*);
template<> ::Im::worldhello* Arena::CreateMaybeMessage<::Im::worldhello>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace Im {

// ===================================================================

class helloworld PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Im.helloworld) */ {
 public:
  inline helloworld() : helloworld(nullptr) {};
  virtual ~helloworld();

  helloworld(const helloworld& from);
  helloworld(helloworld&& from) noexcept
    : helloworld() {
    *this = ::std::move(from);
  }

  inline helloworld& operator=(const helloworld& from) {
    CopyFrom(from);
    return *this;
  }
  inline helloworld& operator=(helloworld&& from) noexcept {
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
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const helloworld& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const helloworld* internal_default_instance() {
    return reinterpret_cast<const helloworld*>(
               &_helloworld_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(helloworld& a, helloworld& b) {
    a.Swap(&b);
  }
  inline void Swap(helloworld* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(helloworld* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline helloworld* New() const final {
    return CreateMaybeMessage<helloworld>(nullptr);
  }

  helloworld* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<helloworld>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const helloworld& from);
  void MergeFrom(const helloworld& from);
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
  void InternalSwap(helloworld* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Im.helloworld";
  }
  protected:
  explicit helloworld(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_msg_2eproto);
    return ::descriptor_table_msg_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStrFieldNumber = 2,
    kIdFieldNumber = 1,
    kOptFieldNumber = 3,
  };
  // string str = 2;
  void clear_str();
  const std::string& str() const;
  void set_str(const std::string& value);
  void set_str(std::string&& value);
  void set_str(const char* value);
  void set_str(const char* value, size_t size);
  std::string* mutable_str();
  std::string* release_str();
  void set_allocated_str(std::string* str);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_str();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_str(
      std::string* str);
  private:
  const std::string& _internal_str() const;
  void _internal_set_str(const std::string& value);
  std::string* _internal_mutable_str();
  public:

  // int32 id = 1;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 opt = 3;
  void clear_opt();
  ::PROTOBUF_NAMESPACE_ID::int32 opt() const;
  void set_opt(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_opt() const;
  void _internal_set_opt(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:Im.helloworld)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr str_;
  ::PROTOBUF_NAMESPACE_ID::int32 id_;
  ::PROTOBUF_NAMESPACE_ID::int32 opt_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_msg_2eproto;
};
// -------------------------------------------------------------------

class worldhello PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Im.worldhello) */ {
 public:
  inline worldhello() : worldhello(nullptr) {};
  virtual ~worldhello();

  worldhello(const worldhello& from);
  worldhello(worldhello&& from) noexcept
    : worldhello() {
    *this = ::std::move(from);
  }

  inline worldhello& operator=(const worldhello& from) {
    CopyFrom(from);
    return *this;
  }
  inline worldhello& operator=(worldhello&& from) noexcept {
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
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const worldhello& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const worldhello* internal_default_instance() {
    return reinterpret_cast<const worldhello*>(
               &_worldhello_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(worldhello& a, worldhello& b) {
    a.Swap(&b);
  }
  inline void Swap(worldhello* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(worldhello* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline worldhello* New() const final {
    return CreateMaybeMessage<worldhello>(nullptr);
  }

  worldhello* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<worldhello>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const worldhello& from);
  void MergeFrom(const worldhello& from);
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
  void InternalSwap(worldhello* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Im.worldhello";
  }
  protected:
  explicit worldhello(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_msg_2eproto);
    return ::descriptor_table_msg_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStrFieldNumber = 2,
    kIdFieldNumber = 1,
    kOtpFieldNumber = 3,
  };
  // string str = 2;
  void clear_str();
  const std::string& str() const;
  void set_str(const std::string& value);
  void set_str(std::string&& value);
  void set_str(const char* value);
  void set_str(const char* value, size_t size);
  std::string* mutable_str();
  std::string* release_str();
  void set_allocated_str(std::string* str);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_str();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_str(
      std::string* str);
  private:
  const std::string& _internal_str() const;
  void _internal_set_str(const std::string& value);
  std::string* _internal_mutable_str();
  public:

  // int32 id = 1;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 otp = 3;
  void clear_otp();
  ::PROTOBUF_NAMESPACE_ID::int32 otp() const;
  void set_otp(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_otp() const;
  void _internal_set_otp(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:Im.worldhello)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr str_;
  ::PROTOBUF_NAMESPACE_ID::int32 id_;
  ::PROTOBUF_NAMESPACE_ID::int32 otp_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_msg_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// helloworld

// int32 id = 1;
inline void helloworld::clear_id() {
  id_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 helloworld::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 helloworld::id() const {
  // @@protoc_insertion_point(field_get:Im.helloworld.id)
  return _internal_id();
}
inline void helloworld::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  id_ = value;
}
inline void helloworld::set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:Im.helloworld.id)
}

// string str = 2;
inline void helloworld::clear_str() {
  str_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& helloworld::str() const {
  // @@protoc_insertion_point(field_get:Im.helloworld.str)
  return _internal_str();
}
inline void helloworld::set_str(const std::string& value) {
  _internal_set_str(value);
  // @@protoc_insertion_point(field_set:Im.helloworld.str)
}
inline std::string* helloworld::mutable_str() {
  // @@protoc_insertion_point(field_mutable:Im.helloworld.str)
  return _internal_mutable_str();
}
inline const std::string& helloworld::_internal_str() const {
  return str_.Get();
}
inline void helloworld::_internal_set_str(const std::string& value) {
  
  str_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void helloworld::set_str(std::string&& value) {
  
  str_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:Im.helloworld.str)
}
inline void helloworld::set_str(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  str_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:Im.helloworld.str)
}
inline void helloworld::set_str(const char* value,
    size_t size) {
  
  str_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:Im.helloworld.str)
}
inline std::string* helloworld::_internal_mutable_str() {
  
  return str_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* helloworld::release_str() {
  // @@protoc_insertion_point(field_release:Im.helloworld.str)
  return str_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void helloworld::set_allocated_str(std::string* str) {
  if (str != nullptr) {
    
  } else {
    
  }
  str_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), str,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:Im.helloworld.str)
}
inline std::string* helloworld::unsafe_arena_release_str() {
  // @@protoc_insertion_point(field_unsafe_arena_release:Im.helloworld.str)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return str_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void helloworld::unsafe_arena_set_allocated_str(
    std::string* str) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (str != nullptr) {
    
  } else {
    
  }
  str_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      str, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:Im.helloworld.str)
}

// int32 opt = 3;
inline void helloworld::clear_opt() {
  opt_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 helloworld::_internal_opt() const {
  return opt_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 helloworld::opt() const {
  // @@protoc_insertion_point(field_get:Im.helloworld.opt)
  return _internal_opt();
}
inline void helloworld::_internal_set_opt(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  opt_ = value;
}
inline void helloworld::set_opt(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_opt(value);
  // @@protoc_insertion_point(field_set:Im.helloworld.opt)
}

// -------------------------------------------------------------------

// worldhello

// int32 id = 1;
inline void worldhello::clear_id() {
  id_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 worldhello::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 worldhello::id() const {
  // @@protoc_insertion_point(field_get:Im.worldhello.id)
  return _internal_id();
}
inline void worldhello::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  id_ = value;
}
inline void worldhello::set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:Im.worldhello.id)
}

// string str = 2;
inline void worldhello::clear_str() {
  str_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& worldhello::str() const {
  // @@protoc_insertion_point(field_get:Im.worldhello.str)
  return _internal_str();
}
inline void worldhello::set_str(const std::string& value) {
  _internal_set_str(value);
  // @@protoc_insertion_point(field_set:Im.worldhello.str)
}
inline std::string* worldhello::mutable_str() {
  // @@protoc_insertion_point(field_mutable:Im.worldhello.str)
  return _internal_mutable_str();
}
inline const std::string& worldhello::_internal_str() const {
  return str_.Get();
}
inline void worldhello::_internal_set_str(const std::string& value) {
  
  str_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void worldhello::set_str(std::string&& value) {
  
  str_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:Im.worldhello.str)
}
inline void worldhello::set_str(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  str_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:Im.worldhello.str)
}
inline void worldhello::set_str(const char* value,
    size_t size) {
  
  str_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:Im.worldhello.str)
}
inline std::string* worldhello::_internal_mutable_str() {
  
  return str_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* worldhello::release_str() {
  // @@protoc_insertion_point(field_release:Im.worldhello.str)
  return str_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void worldhello::set_allocated_str(std::string* str) {
  if (str != nullptr) {
    
  } else {
    
  }
  str_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), str,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:Im.worldhello.str)
}
inline std::string* worldhello::unsafe_arena_release_str() {
  // @@protoc_insertion_point(field_unsafe_arena_release:Im.worldhello.str)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return str_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void worldhello::unsafe_arena_set_allocated_str(
    std::string* str) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (str != nullptr) {
    
  } else {
    
  }
  str_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      str, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:Im.worldhello.str)
}

// int32 otp = 3;
inline void worldhello::clear_otp() {
  otp_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 worldhello::_internal_otp() const {
  return otp_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 worldhello::otp() const {
  // @@protoc_insertion_point(field_get:Im.worldhello.otp)
  return _internal_otp();
}
inline void worldhello::_internal_set_otp(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  otp_ = value;
}
inline void worldhello::set_otp(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_otp(value);
  // @@protoc_insertion_point(field_set:Im.worldhello.otp)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Im

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_msg_2eproto