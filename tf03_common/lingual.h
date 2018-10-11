#ifndef LINGUAL_H
#define LINGUAL_H

#include <QString>
#include <string>
#include <shared_mutex>
#include "export.h"

struct API Lingual {
  QString eng;
  QString chn;
  Lingual& operator+(const QString& str);
  Lingual& operator+(const Lingual& lingual);
};

enum class API Language {
  chinese, english
};

extern Language gCurrentLanguage;
extern std::shared_timed_mutex gCurrentLanguageMutex;

Language API current_language();
void API set_current_language(const Language& language);

QString API which_lingual(const Lingual& lingual);
bool API lingual_equal(const QString& str, const Lingual& lingual);

#endif // LINGUAL_H
