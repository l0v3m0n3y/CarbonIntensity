#ifndef CARBON_INTENSITY_H
#define CARBON_INTENSITY_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class CarbonIntensity {
private:
    std::string api_base = "https://api.carbonintensity.org.uk";
    http_client_config client_config;
    
    http_request create_request(const std::string& endpoint, const std::string& method) {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Accept"), U("application/json"));
        request.headers().add(U("Content-Type"), U("application/json"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        return request;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method) {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    CarbonIntensity() {
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> get_current_intensity() {
        return make_api_call("/intensity", "GET");
    }

    pplx::task<json::value> get_todays_intensity() {
        return make_api_call("/intensity/date", "GET");
    }

    pplx::task<json::value> get_intensity_for_date(const std::string& date) {
        return make_api_call("/intensity/date/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(date))), "GET");
    }

    pplx::task<json::value> get_intensity_for_date_period(const std::string& date, const std::string& period) {
        return make_api_call("/intensity/date/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(date))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(period))), "GET");
    }

    pplx::task<json::value> get_intensity_factors() {
        return make_api_call("/intensity/factors", "GET");
    }

    pplx::task<json::value> get_intensity_for_timepoint(const std::string& from) {
        return make_api_call("/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))), "GET");
    }

    pplx::task<json::value> get_intensity_24h_forward(const std::string& from) {
        return make_api_call("/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw24h", "GET");
    }

    pplx::task<json::value> get_intensity_48h_forward(const std::string& from) {
        return make_api_call("/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw48h", "GET");
    }

    pplx::task<json::value> get_intensity_24h_past(const std::string& from) {
        return make_api_call("/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/pt24h", "GET");
    }

    pplx::task<json::value> get_intensity_between(const std::string& from, const std::string& to) {
        return make_api_call("/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))), "GET");
    }
    
    pplx::task<json::value> get_intensity_stats(const std::string& from, const std::string& to) {
        return make_api_call("/intensity/stats/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))), "GET");
    }

    pplx::task<json::value> get_intensity_stats_block(const std::string& from, const std::string& to, const std::string& block) {
        return make_api_call("/intensity/stats/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(block))), "GET");
    }
    
    pplx::task<json::value> get_current_generation() {
        return make_api_call("/generation", "GET");
    }

    pplx::task<json::value> get_generation_24h_past(const std::string& from) {
        return make_api_call("/generation/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/pt24h", "GET");
    }

    pplx::task<json::value> get_generation_between(const std::string& from, const std::string& to) {
        return make_api_call("/generation/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))), "GET");
    }
    
    pplx::task<json::value> get_regional_current() {
        return make_api_call("/regional", "GET");
    }

    pplx::task<json::value> get_regional_england() {
        return make_api_call("/regional/england", "GET");
    }

    pplx::task<json::value> get_regional_scotland() {
        return make_api_call("/regional/scotland", "GET");
    }

    pplx::task<json::value> get_regional_wales() {
        return make_api_call("/regional/wales", "GET");
    }

    pplx::task<json::value> get_regional_by_postcode(const std::string& postcode) {
        return make_api_call("/regional/postcode/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(postcode))), "GET");
    }

    pplx::task<json::value> get_regional_by_regionid(const std::string& regionid) {
        return make_api_call("/regional/regionid/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(regionid))), "GET");
    }

    pplx::task<json::value> get_regional_24h_forward(const std::string& from) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw24h", "GET");
    }

    pplx::task<json::value> get_regional_24h_forward_postcode(const std::string& from, const std::string& postcode) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw24h/postcode/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(postcode))), "GET");
    }

    pplx::task<json::value> get_regional_24h_forward_regionid(const std::string& from, const std::string& regionid) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw24h/regionid/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(regionid))), "GET");
    }

    pplx::task<json::value> get_regional_48h_forward(const std::string& from) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw48h", "GET");
    }

    pplx::task<json::value> get_regional_48h_forward_postcode(const std::string& from, const std::string& postcode) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw48h/postcode/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(postcode))), "GET");
    }

    pplx::task<json::value> get_regional_48h_forward_regionid(const std::string& from, const std::string& regionid) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/fw48h/regionid/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(regionid))), "GET");
    }

    pplx::task<json::value> get_regional_24h_past(const std::string& from) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/pt24h", "GET");
    }

    pplx::task<json::value> get_regional_24h_past_postcode(const std::string& from, const std::string& postcode) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/pt24h/postcode/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(postcode))), "GET");
    }

    pplx::task<json::value> get_regional_24h_past_regionid(const std::string& from, const std::string& regionid) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/pt24h/regionid/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(regionid))), "GET");
    }

    pplx::task<json::value> get_regional_between(const std::string& from, const std::string& to) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))), "GET");
    }

    pplx::task<json::value> get_regional_between_postcode(const std::string& from, const std::string& to, const std::string& postcode) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))) + "/postcode/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(postcode))), "GET");
    }

    pplx::task<json::value> get_regional_between_regionid(const std::string& from, const std::string& to, const std::string& regionid) {
        return make_api_call("/regional/intensity/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(from))) + "/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(to))) + "/regionid/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(regionid))), "GET");
    }
};

#endif
