#ifndef LINGUAL_H
#define LINGUAL_H

#include <QString>
#include <string>
#include <shared_mutex>

struct Lingual {
  QString eng;
  QString chn;
  Lingual& operator+(const QString& str);
  Lingual& operator+(const Lingual& lingual);
};

enum class Language {
  chinese, english
};

extern Language gCurrentLanguage;
extern std::shared_mutex gCurrentLanguageMutex;

Language current_language();
void set_current_language(const Language& language);

QString which_lingual(const Lingual& lingual);
bool lingual_equal(const QString& str, const Lingual& lingual);

#endif // LINGUAL_H
