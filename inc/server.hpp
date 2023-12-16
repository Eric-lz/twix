#pragma once

#include "instances.hpp"
#include "profiles.hpp"
#include "notifications.hpp"

void threadRecebe(Instances* instances, Profiles* profiles, Notifications* notifications);
void threadEnvia(Instances* instances, Profiles* profiles, Notifications* notifications);