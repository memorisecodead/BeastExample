#pragma once

#include <curl/curl.h>

#include "benchmark/benchmark.h"

size_t write_data_callback(char* ptr, 
    size_t size, size_t nmemb, void* userdata) { return size * nmemb; }

void libcurl_test(benchmark::State& state)
{
    while (state.KeepRunningBatch(state.range_x()))
    {
        for (int i = 0; i < state.range_x(); ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();

            CURL* curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            state.SetIterationTime(elapsed_seconds.count());
        }
    }
}
