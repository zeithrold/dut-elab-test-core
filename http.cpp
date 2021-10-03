//
// Created by Zeithrold on 2021/10/3.
//

#include "extlib/httplib.h"
#include "http.h"
#include "user.h"

using namespace httplib;

void start() {
    Server svr;

    svr.Post("authenticate", [](const Request &req, Response &res ) {

    });
}


