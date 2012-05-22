#ifndef DUT_CONFIG_H
#define DUT_CONFIG_H

#define DUT_EXPORT
#ifdef DUT_EXPORT
#define DUT_API __declspec(dllexport)
#else
#define DUT_API __declspec(dllimport)
#endif // DUT_EXPORT

#endif // DUT_CONFIG_H
