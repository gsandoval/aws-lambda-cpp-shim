#
# Copyright 2017 Alsanium, SAS. or its affiliates. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import json
import os

import runtime

def dump(obj):
    if hasattr(obj, '__slots__'):
        return {slot: getattr(obj, slot) for slot in obj.__slots__}
    return obj.__dict__

class Proxy(object):
    def __getattr__(self, name):
        if name == "init":
            return lambda: None
        runtime.lookup(name)
        return self._handle

    def _handle(self, evt, ctx):
        stringified_event = json.dumps(evt)
        stringified_context = json.dumps(ctx, default=dump)
        stringified_environment = json.dumps({k: v for k, v in ((k, os.getenv(k)) for k in (
                "AWS_ACCESS_KEY_ID",
                "AWS_SECRET_ACCESS_KEY",
                "AWS_SESSION_TOKEN",
                "AWS_SECURITY_TOKEN",
                "_X_AMZN_TRACE_ID",
            )) if v})
        runtime_response = runtime.handle(stringified_event, stringified_context, stringified_environment, ctx.log, ctx.get_remaining_time_in_millis)
        print "runtime_response", runtime_response
        return json.loads(runtime_response)