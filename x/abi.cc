#include <filesystem>
#include <optional>
#include <string>
#include <dlfcn.h>

#include "x/filter.h"
#include "abi.h"

#include "source/common/common/assert.h"
#include "envoy/common/exception.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {
extern "C" {

#define GET_HEADER_VALUE(header_map_type, request_or_response)                                     \
  const std::string_view key_str(static_cast<const char*>(key), key_length);                       \
  const auto header = request_or_response##_headers->get(Http::LowerCaseString(key_str));          \
  if (header.empty()) {                                                                            \
    *_result_buffer_ptr = nullptr;                                                                 \
    *_result_buffer_length_ptr = 0;                                                                \
    return 0;                                                                                      \
  } else {                                                                                         \
    const HeaderEntry* entry = header[0];                                                          \
    const auto entry_length = entry->value().size();                                               \
    *_result_buffer_ptr = const_cast<char*>(entry->value().getStringView().data());                \
    *_result_buffer_length_ptr = entry_length;                                                     \
    return header.size();                                                                          \
  }

size_t __envoy_dynamic_module_v1_http_get_request_header_value(
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr key,
    __envoy_dynamic_module_v1_type_InModuleBufferLength key_length,
    __envoy_dynamic_module_v1_type_DataSlicePtrResult result_buffer_ptr,
    __envoy_dynamic_module_v1_type_DataSliceLengthResult result_buffer_length_ptr) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr* _result_buffer_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSlicePtr*>(result_buffer_ptr);
  __envoy_dynamic_module_v1_type_InModuleBufferLength* _result_buffer_length_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSliceLength*>(result_buffer_length_ptr);

  GET_HEADER_VALUE(RequestHeaderMap, request);
}

size_t __envoy_dynamic_module_v1_http_get_response_header_value(
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr key,
    __envoy_dynamic_module_v1_type_InModuleBufferLength key_length,
    __envoy_dynamic_module_v1_type_DataSlicePtrResult result_buffer_ptr,
    __envoy_dynamic_module_v1_type_DataSliceLengthResult result_buffer_length_ptr) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr* _result_buffer_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSlicePtr*>(result_buffer_ptr);
  __envoy_dynamic_module_v1_type_InModuleBufferLength* _result_buffer_length_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSliceLength*>(result_buffer_length_ptr);
  GET_HEADER_VALUE(ResponseHeaderMap, response);
}

#define GET_HEADER_VALUE_NTH(header_map_type, request_or_response)                                 \
  const std::string_view key_str(static_cast<const char*>(key), key_length);                       \
  const auto header = request_or_response##_headers->get(Http::LowerCaseString(key_str));          \
  if (nth < 0 || nth >= header.size()) {                                                           \
    *_result_buffer_ptr = nullptr;                                                                 \
    *_result_buffer_length_ptr = 0;                                                                \
    return;                                                                                        \
  }                                                                                                \
  const HeaderEntry* entry = header[nth];                                                          \
  const auto entry_length = entry->value().size();                                                 \
  *_result_buffer_ptr = const_cast<char*>(entry->value().getStringView().data());                  \
  *_result_buffer_length_ptr = entry_length;

void __envoy_dynamic_module_v1_http_get_request_header_value_nth(
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr key,
    __envoy_dynamic_module_v1_type_InModuleBufferLength key_length,
    __envoy_dynamic_module_v1_type_DataSlicePtrResult result_buffer_ptr,
    __envoy_dynamic_module_v1_type_DataSliceLengthResult result_buffer_length_ptr, size_t nth) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr* _result_buffer_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSlicePtr*>(result_buffer_ptr);
  __envoy_dynamic_module_v1_type_InModuleBufferLength* _result_buffer_length_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSliceLength*>(result_buffer_length_ptr);
  GET_HEADER_VALUE_NTH(RequestHeaderMap, request);
}

void __envoy_dynamic_module_v1_http_get_response_header_value_nth(
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr key,
    __envoy_dynamic_module_v1_type_InModuleBufferLength key_length,
    __envoy_dynamic_module_v1_type_DataSlicePtrResult result_buffer_ptr,
    __envoy_dynamic_module_v1_type_DataSliceLengthResult result_buffer_length_ptr, size_t nth) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr* _result_buffer_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSlicePtr*>(result_buffer_ptr);
  __envoy_dynamic_module_v1_type_InModuleBufferLength* _result_buffer_length_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSliceLength*>(result_buffer_length_ptr);
  GET_HEADER_VALUE_NTH(ResponseHeaderMap, response);
}

#define GET_BUFFER_SLICES_COUNT(buffer_ptr)                                                        \
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer_ptr);                          \
  return _buffer->getRawSlices(std::nullopt).size();

size_t __envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer) {
  GET_BUFFER_SLICES_COUNT(buffer);
}

size_t __envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer) {
  GET_BUFFER_SLICES_COUNT(buffer);
}

#define GET_BUFFER_SLICE(buffer_ptr, nth)                                                          \
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer_ptr);                          \
  const auto slices = _buffer->getRawSlices(std::nullopt);                                         \
  if (nth < 0 || nth >= slices.size()) {                                                           \
    *_result_buffer_ptr = nullptr;                                                                 \
    *_result_buffer_length_ptr = 0;                                                                \
    return;                                                                                        \
  }                                                                                                \
  *_result_buffer_ptr =                                                                            \
      static_cast<__envoy_dynamic_module_v1_type_DataSlicePtr>(slices[nth].mem_);                  \
  *_result_buffer_length_ptr = slices[nth].len_;

void __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer, size_t nth,
    __envoy_dynamic_module_v1_type_DataSlicePtrResult result_buffer_ptr,
    __envoy_dynamic_module_v1_type_DataSliceLengthResult result_buffer_length_ptr) {
  __envoy_dynamic_module_v1_type_DataSlicePtr* _result_buffer_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSlicePtr*>(result_buffer_ptr);
  __envoy_dynamic_module_v1_type_DataSliceLength* _result_buffer_length_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSliceLength*>(result_buffer_length_ptr);
  GET_BUFFER_SLICE(buffer, nth);
}

void __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer, size_t nth,
    __envoy_dynamic_module_v1_type_DataSlicePtrResult result_buffer_ptr,
    __envoy_dynamic_module_v1_type_DataSliceLengthResult result_buffer_length_ptr) {
  __envoy_dynamic_module_v1_type_DataSlicePtr* _result_buffer_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSlicePtr*>(result_buffer_ptr);
  __envoy_dynamic_module_v1_type_DataSliceLength* _result_buffer_length_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_DataSliceLength*>(result_buffer_length_ptr);
  GET_BUFFER_SLICE(buffer, nth);
}

#define SET_HEADER_VALUE(header_map_type, request_or_response)                                     \
  const std::string key_str(static_cast<const char*>(key), key_length);                            \
  if (value == nullptr) {                                                                          \
    request_or_response##_headers->remove(Http::LowerCaseString(key_str));                         \
    return;                                                                                        \
  }                                                                                                \
  const std::string value_str(static_cast<const char*>(value), value_length);                      \
  request_or_response##_headers->setCopy(Http::LowerCaseString(key_str), value_str);

void __envoy_dynamic_module_v1_http_set_request_header(
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr headers,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr key,
    __envoy_dynamic_module_v1_type_InModuleBufferLength key_length,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr value,
    __envoy_dynamic_module_v1_type_InModuleBufferLength value_length) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  SET_HEADER_VALUE(RequestHeaderMap, request);
}

void __envoy_dynamic_module_v1_http_set_response_header(
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr key,
    __envoy_dynamic_module_v1_type_InModuleBufferLength key_length,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr value,
    __envoy_dynamic_module_v1_type_InModuleBufferLength value_length) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  SET_HEADER_VALUE(ResponseHeaderMap, response);
}

size_t __envoy_dynamic_module_v1_http_get_request_body_buffer_length(
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  return _buffer->length();
}

void __envoy_dynamic_module_v1_http_append_request_body_buffer(
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr data,
    __envoy_dynamic_module_v1_type_InModuleBufferLength data_length) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (data == nullptr || data_length == 0) {
    return;
  }
  std::string_view data_view(static_cast<const char*>(data), data_length);
  _buffer->add(data_view);
}

void __envoy_dynamic_module_v1_http_prepend_request_body_buffer(
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr data,
    __envoy_dynamic_module_v1_type_InModuleBufferLength data_length) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (data == nullptr || data_length == 0) {
    return;
  }
  std::string_view data_view(static_cast<const char*>(data), data_length);
  _buffer->prepend(data_view);
}

void __envoy_dynamic_module_v1_http_drain_request_body_buffer(
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer, size_t length) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (length == 0) {
    return;
  }
  _buffer->drain(length);
}

size_t __envoy_dynamic_module_v1_http_get_response_body_buffer_length(
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  return _buffer->length();
}

void __envoy_dynamic_module_v1_http_append_response_body_buffer(
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr data,
    __envoy_dynamic_module_v1_type_InModuleBufferLength data_length) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (data == nullptr || data_length == 0) {
    return;
  }
  std::string_view data_view(static_cast<const char*>(data), data_length);
  _buffer->add(data_view);
}

void __envoy_dynamic_module_v1_http_prepend_response_body_buffer(
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr data,
    __envoy_dynamic_module_v1_type_InModuleBufferLength data_length) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (data == nullptr || data_length == 0) {
    return;
  }
  std::string_view data_view(static_cast<const char*>(data), data_length);
  _buffer->prepend(data_view);
}

void __envoy_dynamic_module_v1_http_drain_response_body_buffer(
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer, size_t length) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (length == 0) {
    return;
  }
  _buffer->drain(length);
}

void __envoy_dynamic_module_v1_http_continue_request(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr) {
  auto filter = static_cast<HttpFilter*>(envoy_filter_instance_ptr)->shared_from_this();
  auto& dispatcher = filter->decoder_callbacks_->dispatcher();
  dispatcher.post([filter] {
    auto decoder_callbacks = filter->decoder_callbacks_;
    if (decoder_callbacks && !filter->in_continue_) {
      decoder_callbacks->continueDecoding();
      filter->in_continue_ = true;
    }
  });
}

void __envoy_dynamic_module_v1_http_continue_response(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr) {
  auto filter = static_cast<HttpFilter*>(envoy_filter_instance_ptr)->shared_from_this();
  auto& dispatcher = filter->encoder_callbacks_->dispatcher();
  dispatcher.post([filter] {
    auto encoder_callbacks = filter->encoder_callbacks_;
    if (encoder_callbacks && !filter->in_continue_) {
      encoder_callbacks->continueEncoding();
      filter->in_continue_ = true;
    }
  });
}

void __envoy_dynamic_module_v1_http_copy_out_request_body_buffer(
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer, size_t offset, size_t length,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (length == 0) {
    return;
  }
  _buffer->copyOut(offset, length, result_buffer_ptr);
}

void __envoy_dynamic_module_v1_http_copy_out_response_body_buffer(
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer, size_t offset, size_t length,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr) {
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer);
  if (length == 0) {
    return;
  }
  _buffer->copyOut(offset, length, result_buffer_ptr);
}

__envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr
__envoy_dynamic_module_v1_http_get_request_body_buffer(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr) {
  auto filter = static_cast<HttpFilter*>(envoy_filter_instance_ptr);
  if (filter->decoder_callbacks_) {
    auto buf = const_cast<Buffer::Instance*>(filter->decoder_callbacks_->decodingBuffer());
    return static_cast<__envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr>(buf);
  }
  return nullptr;
}

__envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr
__envoy_dynamic_module_v1_http_get_response_body_buffer(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr) {
  auto filter = static_cast<HttpFilter*>(envoy_filter_instance_ptr);
  if (filter->encoder_callbacks_) {
    auto buf = const_cast<Buffer::Instance*>(filter->encoder_callbacks_->encodingBuffer());
    return static_cast<__envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr>(buf);
  }
  return nullptr;
}

void __envoy_dynamic_module_v1_http_send_response(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    uint32_t status_code, __envoy_dynamic_module_v1_type_InModuleHeadersPtr headers_vector,
    __envoy_dynamic_module_v1_type_InModuleHeadersSize headers_vector_size,
    __envoy_dynamic_module_v1_type_InModuleBufferPtr body_ptr,
    __envoy_dynamic_module_v1_type_InModuleBufferLength body_length) {
  auto filter = static_cast<HttpFilter*>(envoy_filter_instance_ptr);

  std::function<void(ResponseHeaderMap & headers)> modify_headers = nullptr;
  if (headers_vector != 0 && headers_vector_size != 0) {
    modify_headers = [headers_vector, headers_vector_size](ResponseHeaderMap& headers) {
      auto headers_ptr =
          reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleHeader*>(headers_vector);

      for (size_t i = 0; i < headers_vector_size; i++) {
        const auto& header = &headers_ptr[i];
        const std::string_view key(static_cast<const char*>(header->header_key),
                                   header->header_key_length);
        const std::string_view value(static_cast<const char*>(header->header_value),
                                     header->header_value_length);
        headers.setCopy(Http::LowerCaseString(key), std::string(value));
      }
    };
  }
  const std::string_view body =
      body_ptr ? std::string_view(static_cast<const char*>(body_ptr), body_length) : "";

  if (filter->decoder_callbacks_) {
    filter->decoder_callbacks_->sendLocalReply(static_cast<Http::Code>(status_code), body,
                                               modify_headers, 0, "dynamic_module");
  } else if (filter->encoder_callbacks_) {
    filter->encoder_callbacks_->sendLocalReply(static_cast<Http::Code>(status_code), body,
                                               modify_headers, 0, "dynamic_module");
  }
}

} // extern "C"

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
