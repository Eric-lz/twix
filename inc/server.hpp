#pragma once

#include "instances.hpp"
#include "profiles.hpp"
#include "notifications.hpp"

void threadSession(UDP* udp, Instances* instances, Notifications* notifications);
void threadKeepAlive(UDP* udp, Instances* inst);
