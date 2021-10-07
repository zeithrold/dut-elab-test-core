//
// Created by Zeithrold on 2021/10/3.
//

#include "extlib/cpp-httplib/httplib.h"
#include "extlib/configor/include/configor/json.hpp"
#include <cstdio>
#include "http.h"
#include "data.h"

using namespace std;
using namespace httplib;
using namespace configor;

void start() {
    Server svr;

    svr.Post("/authenticate", [](const Request &req, Response &res ) {

    });
    svr.Post("/refresh", [](const Request &req, Response &res) {
        string access_token = req.get_header_value("Authentication");
        if (access_token.empty()) {
            res.status = 403;
            const json result = {
                    "error", "AccessTokenNullException"
            };
            res.set_content(result.dump(), "application/json");
        }
        bool success = dutelab::user_refresh(req.get_header_value("Authentication"));
        const json result = {
                "success", success
        };
        res.set_content(result.dump(), "application/json");
    });
}


