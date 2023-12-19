#pragma once

#include "instances.hpp"
#include "profiles.hpp"
#include "notifications.hpp"

void threadSession(UDP* udp, sockaddr_in cliaddr, Profiles* profiles, Notifications* notifications);
//void threadEnvia(Instances* instances, Profiles* profiles, Notifications* notifications);