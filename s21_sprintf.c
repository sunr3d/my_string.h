#include "s21_sprintf.h"

int s21_sprintf(char* str, const char* format, ...) {
  struct s21_format* parsed_format = format_init();
  int n = read_format(format, parsed_format);

  va_list factor;
  va_start(factor, format);

  int current_output_pos = 0;
  int j = 0;
  for (int i = 0; i < n; i++) {
    for (; j < parsed_format->specs[i]->pos_start; j++) {
      str[current_output_pos++] = format[j];
    }
    print_spec(str, &factor, parsed_format->specs[i], &current_output_pos);
    j = parsed_format->specs[i]->pos_end + 1;
  }

  char c;
  while ((c = format[j]) != '\0') {
    str[current_output_pos++] = format[j++];
  }
  str[current_output_pos] = '\0';

  va_end(factor);
  format_free(parsed_format);

  return current_output_pos;
}

int read_format(const char* format, struct s21_format* parsed_format) {
  int i = 0;
  char c;
  while ((c = format[i++]) != '\0') {
    if (c == '%') {
      struct s21_spec* spec = spec_init(i - 1);
      format_addspec(parsed_format, spec);
      parse_format(format, spec, &i, &c);
    }
  }
  return parsed_format->speccnt;
}

void parse_format(const char* format, struct s21_spec* spec, int* i, char* c) {
  struct s21_parser* parser = parser_init();
  while (!parser->end && (*c = format[(*i)++]) != '\0') {
    switch (*c) {
      case '-':
      case '+':
      case ' ':
        spec_set_flag(spec, *c);
        break;
      case 'l':
        spec->l = 1;
        break;
      case 'h':
        spec->h = 1;
        break;
      case 'c':
      case 'd':
      case 'f':
      case 's':
      case 'u':
        parser->first_num = FALSE;
        spec_set_type(spec, *c);
        spec->pos_end = *i - 1;
        parser->end = 1;
        break;
      case '.':
        parser->first_num = FALSE;
        parser->point = 1;
        spec->accuracy = 0;
        spec->accuracy_set = TRUE;
        break;
      default:
        case_number(spec, c, parser);
        break;
    }
  }
  set_width_accuracy(spec, parser);
  free(parser);
}

void case_number(struct s21_spec* spec, char* c, struct s21_parser* parser) {
  if (*c >= '0' && *c <= '9') {
    if (parser->first_num && *c == '0') {
      spec_set_fill_0(spec);
    } else if (!parser->point) {
      parser->w_set = TRUE;
      parser->w = parser->w * 10 + (*c - '0');
    } else {
      parser->accur_set = TRUE;
      parser->accur = parser->accur * 10 + (*c - '0');
    }
  }
  parser->first_num = FALSE;
}

void set_width_accuracy(struct s21_spec* spec, struct s21_parser* parser) {
  if (parser->w_set) spec_set_w(spec, parser->w);
  if (parser->accur_set) spec_set_accur(spec, parser->accur);
}

void print_spec(char* str, va_list* factor, struct s21_spec* spec,
                int* current_output_pos) {
  enum s21_spec_type type = spec->type;
  char res_char;
  wchar_t res_wchar;
  int res_int;
  long int res_long;
  unsigned int res_uint;
  unsigned long int res_ulong;
  char* res_str;
  wchar_t* res_wstr;
  double res_double;

  switch (type) {
    case TYPE_C:
      res_char = (char)va_arg(*factor, int);
      if (spec->flag != '-') fill_min_width(str, current_output_pos, spec, 1);
      print_c(str, current_output_pos, res_char);
      if (spec->flag == '-') fill_min_width(str, current_output_pos, spec, 1);
      break;
    case TYPE_HD:
    case TYPE_D:
      res_int = va_arg(*factor, int);
      print_d(str, spec, current_output_pos, res_int);
      break;
    case TYPE_S:
      res_str = (char*)va_arg(*factor, char*);
      print_s(str, spec, current_output_pos, res_str);
      break;
    case TYPE_F:
      res_double = (double)va_arg(*factor, double);
      print_f(str, spec, current_output_pos, res_double);
      break;
    case TYPE_HU:
    case TYPE_U:
      res_uint = (unsigned int)va_arg(*factor, unsigned int);
      print_u(str, spec, current_output_pos, res_uint);
      break;
    case TYPE_LD:
      res_long = (long int)va_arg(*factor, long int);
      print_ld(str, spec, current_output_pos, res_long);
      break;
    case TYPE_LU:
      res_ulong = (unsigned long int)va_arg(*factor, unsigned long int);
      print_lu(str, spec, current_output_pos, res_ulong);
      break;
    case TYPE_LS:
      res_wstr = (wchar_t*)va_arg(*factor, wchar_t*);
      print_ls(str, spec, current_output_pos, res_wstr);
      break;
    case TYPE_LC:
      res_wchar = (wchar_t)va_arg(*factor, wchar_t);
      if (spec->flag != '-') fill_min_width(str, current_output_pos, spec, 1);
      print_lc(str, current_output_pos, res_wchar);
      if (spec->flag == '-') fill_min_width(str, current_output_pos, spec, 1);
      break;
  }
}

void print_c(char* str, int* current_output_pos, char c) {
  str[(*current_output_pos)++] = c;
}

void print_lc(char* str, int* current_output_pos, wchar_t c) {
  char buff[MB_CUR_MAX];
  int len = wctomb(buff, c);
  if (len > 0) {
    for (int i = 0; i < len; i++) {
      str[(*current_output_pos)++] = buff[i];
    }
  }
}

void print_d(char* str, struct s21_spec* spec, int* current_output_pos, int d) {
  char* c = (char*)malloc(15 * sizeof(char));
  int minus = FALSE;
  if (d < 0) {
    d = -d;
    minus = TRUE;
  }

  int count = uint_to_chars(c, d);
  print_uint_chars(c, str, current_output_pos, spec, count, minus);
  free(c);
}

void print_ld(char* str, struct s21_spec* spec, int* current_output_pos,
              long d) {
  char* c = (char*)malloc(25 * sizeof(char));
  int minus = FALSE;
  if (d < 0) {
    d = -d;
    minus = TRUE;
  }

  int count = ulong_to_chars(c, d);
  print_uint_chars(c, str, current_output_pos, spec, count, minus);
  free(c);
}

void print_s(char* str, struct s21_spec* spec, int* current_output_pos,
             char* s) {
  int len = s21_strlen(s);

  int count = len;
  if (spec->accuracy_set && spec->accuracy < len) {
    count = spec->accuracy;
  }

  if (spec->flag != '-') {
    fill_min_width(str, current_output_pos, spec, count);
  }

  for (int i = 0; i < count; i++) {
    str[(*current_output_pos)++] = s[i];
  }

  if (spec->flag == '-') {
    fill_min_width(str, current_output_pos, spec, count);
  }
}

void print_ls(char* str, struct s21_spec* spec, int* current_output_pos,
              wchar_t* s) {
  int len = wcslen(s);

  int count = len;
  if (spec->accuracy_set && spec->accuracy < len) {
    count = spec->accuracy;
  }

  if (spec->flag != '-') {
    fill_min_width(str, current_output_pos, spec, count);
  }

  for (int i = 0; i < count; i++) {
    print_lc(str, current_output_pos, s[i]);
  }

  if (spec->flag == '-') {
    fill_min_width(str, current_output_pos, spec, count);
  }
}

void print_u(char* str, struct s21_spec* spec, int* current_output_pos,
             unsigned int u) {
  char* c = (char*)malloc(15 * sizeof(char));
  int count = uint_to_chars(c, u);
  print_uint_chars(c, str, current_output_pos, spec, count, FALSE);
  free(c);
}

void print_lu(char* str, struct s21_spec* spec, int* current_output_pos,
              unsigned long u) {
  char* c = (char*)malloc(25 * sizeof(char));
  int count = ulong_to_chars(c, u);
  print_uint_chars(c, str, current_output_pos, spec, count, FALSE);
  free(c);
}

void print_f(char* str, struct s21_spec* spec, int* current_output_pos,
             double f) {
  char* exp = (char*)malloc(10 * sizeof(char));
  char* frac = (char*)malloc(53 * sizeof(char));
  int minus = FALSE;
  if (f < 0) {
    minus = TRUE;
    f = -f;
  }
  f = round_to_decimal_places(f, spec->accuracy);
  int exp_part = (int)f;
  int exp_count = uint_to_chars(exp, exp_part);
  int frac_count = fract_to_chars(frac, f, spec);
  int period = 1;
  if (!spec->accuracy) {
    period = 0;
  }
  if (spec->flag != '-' && !spec->fill_0) {
    fill_min_width(str, current_output_pos, spec,
                   minus + exp_count + period + spec->accuracy);
  }

  if (minus) {
    str[(*current_output_pos)++] = '-';
  } else if (spec->flag == '+') {
    str[(*current_output_pos)++] = '+';
  } else if (spec->flag == ' ') {
    str[(*current_output_pos)++] = ' ';
  }

  if (spec->flag != '-' && spec->fill_0) {
    fill_min_width(str, current_output_pos, spec,
                   minus + exp_count + period + spec->accuracy);
  }

  for (int i = exp_count - 1; i >= 0; i--) {
    str[(*current_output_pos)++] = exp[i];
  }
  if (spec->accuracy) str[(*current_output_pos)++] = '.';
  for (int i = 0; i < frac_count; i++) {
    str[(*current_output_pos)++] = frac[i];
    str[*current_output_pos] = '\0';
  }

  for (int i = spec->accuracy - frac_count - 1; i >= 0; i--) {
    str[(*current_output_pos)++] = '0';
  }

  if (spec->flag == '-') {
    fill_min_width(str, current_output_pos, spec,
                   minus + exp_count + period + spec->accuracy);
  }
  str[*current_output_pos] = '\0';
  free(exp);
  free(frac);
}

void spec_set_type(struct s21_spec* s_spec, char c) {
  switch (c) {
    case 'c':
      if (s_spec->l == 1) {
        s_spec->type = TYPE_LC;
        s_spec->l = 0;
      } else
        s_spec->type = TYPE_C;
      break;
    case 'd':
      if (s_spec->l == 1) {
        s_spec->type = TYPE_LD;
        s_spec->l = 0;
      } else if (s_spec->h == 1) {
        s_spec->type = TYPE_HD;
        s_spec->h = 0;
      } else
        s_spec->type = TYPE_D;
      break;
    case 'f':
      s_spec->type = TYPE_F;
      break;
    case 's':
      if (s_spec->l == 1) {
        s_spec->type = TYPE_LS;
        s_spec->l = 0;
      } else
        s_spec->type = TYPE_S;
      break;
    case 'u':
      if (s_spec->l == 1) {
        s_spec->type = TYPE_LU;
        s_spec->l = 0;
      } else if (s_spec->h == 1) {
        s_spec->type = TYPE_HU;
        s_spec->h = 0;
      } else
        s_spec->type = TYPE_U;
      break;
  }
}

void spec_set_flag(struct s21_spec* s_spec, char c) { s_spec->flag = c; }

void spec_set_fill_0(struct s21_spec* s_spec) { s_spec->fill_0 = 1; }

void spec_set_w(struct s21_spec* s_spec, int width) {
  s_spec->min_w = width;
  s_spec->min_w_set = TRUE;
}

void spec_set_accur(struct s21_spec* s_spec, int accuracy) {
  s_spec->accuracy = accuracy;
  s_spec->accuracy_set = TRUE;
}

double round_to_decimal_places(double value, int decimal_places) {
  double factor = pow(10.0, decimal_places);
  return round(value * factor) / factor;
}

int uint_to_chars(char* c, unsigned int d) {
  int count = 0;
  while (d > 9) {
    c[count++] = (d % 10) + '0';
    d = d / 10;
  }
  c[count++] = d + '0';
  return count;
}

int ulong_to_chars(char* c, unsigned long d) {
  int count = 0;
  while (d > 9) {
    c[count++] = (d % 10) + '0';
    d = d / 10;
  }
  c[count++] = d + '0';
  return count;
}

int fract_to_chars(char* c, double f, struct s21_spec* spec) {
  int count = 0;
  int exp_part = (int)f;
  f = round_to_decimal_places((f - exp_part) * 10, spec->accuracy);

  while (f > 0) {
    count++;

    int accur = spec->accuracy;
    if (spec->accuracy_set && count >= accur) {
      if (count == accur) {
        c[count - 1] = round(f) + '0';
      }
      break;
    } else {
      exp_part = (int)f;
      c[count - 1] = exp_part + '0';
      f = round_to_decimal_places((f - exp_part) * 10, spec->accuracy);
    }
  }

  return count;
}

void print_uint_chars(char* from, char* to, int* current_output_pos,
                      struct s21_spec* spec, int count, int minus) {
  int ac_0_count = 0;
  if (spec->accuracy_set && count < spec->accuracy) {
    ac_0_count = spec->accuracy - count;
  }
  int flag = minus;
  if (spec->flag == '+') {
    flag = 1;
  }
  if (spec->flag != '-' && (!spec->fill_0 || spec->accuracy_set)) {
    fill_min_width(to, current_output_pos, spec, ac_0_count + count + flag);
  }

  if (minus) {
    to[(*current_output_pos)++] = '-';
  } else if (spec->flag == '+') {
    to[(*current_output_pos)++] = '+';
  }

  if (spec->flag != '-' && (spec->fill_0 && !spec->accuracy_set)) {
    fill_min_width(to, current_output_pos, spec, ac_0_count + count + flag);
  }

  for (int i = 0; i < ac_0_count; i++) {
    to[(*current_output_pos)++] = '0';
  }
  for (int i = count - 1; i >= 0; i--) {
    to[(*current_output_pos)++] = from[i];
  }

  if (spec->flag == '-') {
    fill_min_width(to, current_output_pos, spec, ac_0_count + count + flag);
  }
}

void fill_min_width(char* str, int* current_output_pos, struct s21_spec* spec,
                    int count) {
  if (spec->min_w_set && spec->min_w > count) {
    char filler = ' ';
    if (spec->fill_0 && spec->type != TYPE_C && spec->type != TYPE_S &&
        (!spec->accuracy_set || spec->type == TYPE_F)) {
      filler = '0';
    }
    for (int i = 0; i < spec->min_w - count; i++) {
      str[(*current_output_pos)++] = filler;
    }
  }
}

struct s21_format* format_init() {
  struct s21_format* format = malloc(sizeof(struct s21_format));
  format->speccnt = 0;
  format->specssize = 10;
  format->specs = calloc(format->specssize, sizeof(struct s21_spec*));
  return format;
}

struct s21_spec* spec_init(int pos_start) {
  struct s21_spec* spec = malloc(sizeof(struct s21_spec));
  spec->type = TYPE_C;
  spec->pos_start = pos_start;
  spec->pos_end = pos_start;
  spec->flag = '0';
  spec->min_w = 0;
  spec->min_w_set = FALSE;
  spec->accuracy = 6;
  spec->accuracy_set = FALSE;
  spec->h = 0;
  spec->l = 0;
  spec->fill_0 = 0;

  return spec;
}

void format_addspec(struct s21_format* format, struct s21_spec* spec) {
  format->speccnt++;
  if (format->specssize < format->speccnt) {
    format->specssize += 10;
    format->specs =
        realloc(format->specs, format->specssize * sizeof(struct s21_spec*));
  }
  format->specs[format->speccnt - 1] = spec;
}

struct s21_parser* parser_init() {
  struct s21_parser* parser = malloc(sizeof(struct s21_parser));
  parser->end = 0;
  parser->w = 0;
  parser->accur = 0;
  parser->point = 0;
  parser->w_set = FALSE;
  parser->accur_set = FALSE;
  parser->first_num = TRUE;
  return parser;
}

void format_free(struct s21_format* format) {
  for (int i = 0; i < format->speccnt; i++) {
    free(format->specs[i]);
  }
  free(format->specs);
  free(format);
}