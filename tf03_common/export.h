#ifndef TF03_COMMON_EXPORT_H
#define TF03_COMMON_EXPORT_H

#ifdef _WIN32
#	ifdef tf03_common_EXPORTS
#		define API __declspec(dllexport)
#	else
#		define API __declspec(dllimport)
#	endif
#else
#	define API
#endif // WIN32

#endif // TF03_COMMON_EXPORT_H
