#pragma once
#define RRE_DISABLE_COPY(Class) \
Class(const Class&); \
Class& operator=(const Class&);