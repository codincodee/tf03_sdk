#include "lingual.h"

Lingual& Lingual::operator+(const QString& str) {
  eng += str;
  chn += str;
  return *this;
}

Lingual& Lingual::operator+(const Lingual& lingual) {
  eng += lingual.eng;
  chn += lingual.chn;
  return *this;
}

Language gCurrentLanguage = Language::chinese;
std::shared_timed_mutex gCurrentLanguageMutex;

Language current_language() {
  std::shared_lock<std::shared_timed_mutex> lock(gCurrentLanguageMutex);
  return gCurrentLanguage;
}

void set_current_language(const Language &language) {
  std::unique_lock<std::shared_timed_mutex> lock(gCurrentLanguageMutex);
  gCurrentLanguage = language;
}

QString which_lingual(const Lingual &lingual) {
  auto language = current_language();
  switch (language) {
  case Language::chinese : return lingual.chn;
  case Language::english : return lingual.eng;
  }
  return "";
}

bool lingual_equal(const QString &str, const Lingual &lingual) {
  return str == which_lingual(lingual);
}
