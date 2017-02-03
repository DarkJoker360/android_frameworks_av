/*
 * Copyright 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "WOmxObserver.h"

#include <vector>

#include <cutils/native_handle.h>
#include <frameworks/native/include/binder/Binder.h>

#include "Conversion.h"

namespace android {
namespace hardware {
namespace media {
namespace omx {
namespace V1_0 {
namespace implementation {

// LWOmxObserver
LWOmxObserver::LWOmxObserver(sp<IOmxObserver> const& base) : mBase(base) {
}

void LWOmxObserver::onMessages(std::list<omx_message> const& lMessages) {
    hidl_vec<Message> tMessages;
    std::vector<native_handle_t*> handles(lMessages.size());
    tMessages.resize(lMessages.size());
    size_t i = 0;
    for (auto const& message : lMessages) {
        wrapAs(&tMessages[i], &handles[i], message);
        ++i;
    }
    mBase->onMessages(tMessages);
    for (auto& handle : handles) {
        native_handle_close(handle);
        native_handle_delete(handle);
    }
}

::android::IBinder* LWOmxObserver::onAsBinder() {
    return nullptr;
}

// TWOmxObserver
TWOmxObserver::TWOmxObserver(sp<IOMXObserver> const& base) : mBase(base) {
}

Return<void> TWOmxObserver::onMessages(const hidl_vec<Message>& tMessages) {
    std::list<omx_message> lMessages;
    for (size_t i = 0; i < tMessages.size(); ++i) {
        lMessages.push_back(omx_message{});
        convertTo(&lMessages.back(), tMessages[i]);
    }
    mBase->onMessages(lMessages);
    return Return<void>();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace omx
}  // namespace media
}  // namespace hardware
}  // namespace android
