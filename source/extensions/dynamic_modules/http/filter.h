
#pragma once

#include <string>

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include "source/extensions/dynamic_modules/http/config.pb.h"
#include "source/extensions/dynamic_modules/http/http_dynamic_module.h"

namespace Envoy {
namespace Extensions {
namespace DynamicModules {
namespace Http {

using namespace Envoy::Http;

/**
 * A filter that uses a dynamic module and corresponds to a single filter instance.
 */
class HttpFilter : public Http::StreamFilter, public std::enable_shared_from_this<HttpFilter> {
public:
  HttpFilter(HttpDynamicModuleSharedPtr);
  ~HttpFilter() override;

  /**
   * Ensure that the in-module http filter instance is initialized. This is called by
   * decodeHeaders() to ensure that it is initialized before calling into the * dynamic module.
   * Note: this is made public for testing purposes.
   */
  void ensureHttpFilterInstance();

  /**
   * Destroy the in-module http filter. This is called by onDestroy() and the destructor to ensure
   * that the it is destroyed before the dynamic module is unloaded. Note: this is made public for
   * testing purposes.
   */
  void destoryHttpFilterInstance();

  // N.B. The event hooks inlined here are not supported by the dynamic modules for now.

  // ---------- Http::StreamFilterBase ------------

  void onStreamComplete() override{};

  /**
   * This routine is called prior to a filter being destroyed. This may happen after normal stream
   * finish (both downstream and upstream) or due to reset. Every filter is responsible for making
   * sure that any async events are cleaned up in the context of this routine. This includes timers,
   * network calls, etc. The reason there is an onDestroy() method vs. doing this type of cleanup
   * in the destructor is due to the deferred deletion model that Envoy uses to avoid stack unwind
   * complications. Filters must not invoke either encoder or decoder filter callbacks after having
   * onDestroy() invoked. Filters that cross-register as access log handlers receive log() before
   * onDestroy().
   */
  void onDestroy() override;

  // ----------  Http::StreamDecoderFilter  ----------
  /**
   * Called with decoded headers, optionally indicating end of stream.
   * @param headers supplies the decoded headers map.
   * @param end_stream supplies whether this is a header only request/response.
   * @return FilterHeadersStatus determines how filter chain iteration proceeds.
   */
  FilterHeadersStatus decodeHeaders(RequestHeaderMap& headers, bool end_stream) override;

  /**
   * Called with a decoded data frame.
   * @param data supplies the decoded data.
   * @param end_stream supplies whether this is the last data frame.
   * Further note that end_stream is only true if there are no trailers.
   * @return FilterDataStatus determines how filter chain iteration proceeds.
   */
  FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;

  FilterTrailersStatus decodeTrailers(RequestTrailerMap&) override {
    return FilterTrailersStatus::Continue;
  }

  FilterMetadataStatus decodeMetadata(MetadataMap&) override {
    return FilterMetadataStatus::Continue;
  };

  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override {
    decoder_callbacks_ = &callbacks;
  }

  void decodeComplete() override{};

  // ----------  Http::StreamEncoderFilter  ----------

  Filter1xxHeadersStatus encode1xxHeaders(ResponseHeaderMap&) override {
    return Filter1xxHeadersStatus::Continue;
  };

  /**
   * Called with headers to be encoded, optionally indicating end of stream.
   *
   * The only 1xx that may be provided to encodeHeaders() is a 101 upgrade, which will be the final
   * encodeHeaders() for a response.
   *
   * @param headers supplies the headers to be encoded.
   * @param end_stream supplies whether this is a header only request/response.
   * @return FilterHeadersStatus determines how filter chain iteration proceeds.
   */
  FilterHeadersStatus encodeHeaders(ResponseHeaderMap& headers, bool end_stream) override;

  /**
   * Called with data to be encoded, optionally indicating end of stream.
   * @param data supplies the data to be encoded.
   * @param end_stream supplies whether this is the last data frame.
   * Further note that end_stream is only true if there are no trailers.
   * @return FilterDataStatus determines how filter chain iteration proceeds.
   */
  FilterDataStatus encodeData(Buffer::Instance& data, bool end_stream) override;

  FilterTrailersStatus encodeTrailers(ResponseTrailerMap&) override {
    return FilterTrailersStatus::Continue;
  };

  FilterMetadataStatus encodeMetadata(MetadataMap&) override {
    return FilterMetadataStatus::Continue;
  };

  void setEncoderFilterCallbacks(StreamEncoderFilterCallbacks& callbacks) override {
    encoder_callbacks_ = &callbacks;
  }

  void encodeComplete() override{};

public:
  // The callbacks for the filter. They are only valid until onDestroy() is called.
  StreamDecoderFilterCallbacks* decoder_callbacks_ = nullptr;
  StreamEncoderFilterCallbacks* encoder_callbacks_ = nullptr;

  // The in-module per-stream http filter instance for the module.
  void* http_filter_instance_ = nullptr;

  // If the filter is in the continue state. This is to avoid assertion failure on prohitbiting
  // calling coninueDecoding() or continueEncoding() multiple times.
  bool in_continue_ = false;

private:
  const HttpDynamicModuleSharedPtr dynamic_module_ = nullptr;
};

} // namespace Http
} // namespace DynamicModules
} // namespace Extensions
} // namespace Envoy
