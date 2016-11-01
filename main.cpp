#include <vector>
#include <iostream>
#include <cassert>
#include <cctype>
#include <sstream>
#include <string>
#include <fstream>

//From http://www.richelbilderbeek.nl/CppStrToUpper.htm
std::string to_upper(std::string s) noexcept
{
  for (char& c: s) { c = std::toupper(c); }
  return s;
}


/*
std::vector<std::string> get_codons(const char c)
{
  switch(c)
  {
    case 'A': return { "GCT", "GCC", "GCA", "GCG" };
    case 'C': return { "TGT", "TGC" };
    case 'D': return { "GAT", "GAC" };
    case 'E': return { "GAA", "GAG" };
    case 'F': return { "TTT", "TTC" };
    case 'G': return { "GGT", "GGC", "GGA", "GGG" };
    case 'H': return { "CAT", "CAC" };
    case 'I': return { "ATT", "ATC", "ATA" };
    case 'K': return { "AAA", "AAG" };
    case 'L': return { "TTA", "TTG", "CTT", "CTC", "CTA", "CTG" };
    case 'N': return { "AAT", "AAC" };
    case 'M': return { "ATG" };
    case 'P': return { "CCT", "CCC", "CCA", "CCG" };
    case 'Q': return { "CAA", "CAG" };
    case 'R': return { "CGT", "CGC", "CGA", "CGG", "AGA", "AGG" };
    case 'S': return { "TCT", "TCC", "TCA", "TCG", "AGT", "AGC" };
    case 'T': return { "ACT", "ACC", "ACA", "ACG" };
    case 'V': return { "GTT", "GTC", "GTA", "GTG" };
    case 'W': return { "TGG" };
    case 'Y': return { "TAT", "TAC" };
    //Stopcodon
    case 'X': return { "TAA", "TGA", "TAG" };
    //Startcodon
    case ' ': return { "ATG" };
    default: return { "" };
  }
}
*/

std::string get_codon_wildcard(const char c)
{
  switch(c)
  {
    case 'A': return "GC*";
    case 'C': return "TG(CT)";
    case 'D': return "GA(CT)";
    case 'E': return "GA(AG)";
    case 'F': return "TT(TC)";
    case 'G': return "GG*";
    case 'H': return "CA(CT)";
    case 'I': return "AT(TCA)";
    case 'K': return "AA(AG)";
    case 'L': return "TT(AG)|CT*";
    case 'N': return "AA(CT)";
    case 'M': return "ATG";
    case 'P': return "CC*";
    case 'Q': return "CA(AG)";
    case 'R': return "CG*|AG(AG)";
    case 'S': return "TC*|AG(CT)";
    case 'T': return "AC*";
    case 'V': return "GT*";
    case 'W': return "TGG";
    case 'Y': return "TA(CT)";
    //Stopcodon
    case 'X': return "T(AA|GA|AG)";
    //Startcodon
    case ' ': return "ATG";
    default: return "";
  }
}

std::string get_codon(const char c)
{
  switch(c)
  {
    case 'A': return "GCA";
    case 'C': return "TGC";
    case 'D': return "GAC";
    case 'E': return "GAA";
    case 'F': return "TTT";
    case 'G': return "GGA";
    case 'H': return "CAC";
    case 'I': return "ATT";
    case 'K': return "AAA";
    case 'L': return "TTA";
    case 'N': return "AAC";
    case 'M': return "ATG";
    case 'P': return "CCA";
    case 'Q': return "CAA";
    case 'R': return "CGA";
    case 'S': return "TCA";
    case 'T': return "ACA";
    case 'V': return "GTA";
    case 'W': return "TGG";
    case 'Y': return "TAC";
    //Stopcodon
    case 'X': return "TAA";
    //Startcodon
    case ' ': return "ATG";
    default: return "   ";
  }
}

std::string get_codons(const std::string& s)
{
  std::string t;
  for (const auto c: s) { t += get_codon(c); }
  return t;
}

std::vector<std::string> get_group_name() noexcept
{
  return {
    "Theoreticaly",
    "Evolutionary",
    "  Community ",
    " Ecology    " };
}

std::vector<std::string> get_group_name_uppercase() noexcept
{
  std::vector<std::string> v = get_group_name();
  for (std::string& s: v) { s = to_upper(s); }
  return v;
}

std::vector<std::string> get_group_codons() noexcept
{
  std::vector<std::string> v = get_group_name_uppercase();
  for (std::string& s: v) { s = get_codons(s); }
  return v;
}

int get_distance(const std::string& s, const std::string& t)
{
  const int min_size{static_cast<int>(std::min(s.size(), t.size()))};
  const int max_size{static_cast<int>(std::max(s.size(), t.size()))};
  int distance{max_size - min_size};
  for (int i=0; i!=min_size; ++i)
  {
    if (s[i] == ' ' && t[i] == ' ') continue;
    distance += s[i] != t[i] ? 1 : 0;
  }
  return distance;
}

bool is_okay(const std::vector<std::string>& v)
{
  assert(v.size() == 4);
  //1 must be closest to 2
  const auto a = v[0];
  const auto b = v[1];
  const auto c = v[2];
  const auto d = v[3];
  return
       get_distance(a, b) < get_distance(a, c)
    && get_distance(a, b) < get_distance(a, d)
    && get_distance(a, b) < get_distance(b, c)
    && get_distance(a, b) < get_distance(b, d)
    && get_distance(c, d) < get_distance(c, a)
    && get_distance(c, d) < get_distance(c, b)
    && get_distance(c, d) < get_distance(d, a)
    && get_distance(c, d) < get_distance(d, b)
  ;
}

template <class T>
std::string get_svg_line(const T x1, const T y1, const T x2, const T y2, const int stroke_width = 1)
{
  std::stringstream s;
  s << "  <line "
    << "x1=\""<< x1 << "\" "
    << "y1=\""<< y1 << "\" "
    << "x2=\""<< x2 << "\" "
    << "y2=\""<< y2 << "\" "
    << "style=\"stroke:black;stroke-width:" << stroke_width << "\" />\n"
  ;
  return s.str();
}

std::string create_svg_string()
{
  std::stringstream s;
  s << "<svg height=\"500\" width=\"1500\">\n";

  const int offset_x{200};
  //Show name
  {
    const int offset_y{50};
    const int width{40};
    const int height{40};
    const auto v = get_group_name();
    for (int y=0; y!=4; ++y)
    {
      const int sz = v[y].size();
      for (int x=0; x!=sz; ++x)
      {
        const int x_co = offset_x + (x * width);
        const int y_co = offset_y + (y * height);
        s << "  <rect x=\""<< (x_co - 5) << "\" y=\""<< (y_co + 10 - height) << "\" width=\"" << width << "\" height=\"" << height << "\"  style=\"fill:white;stroke:black;stroke-width:2;opacity:1\" />\n";
        s << "  <text x=\""<< x_co << "\" y=\""<< y_co << "\" font-family=\"Verdana\" font-size=\"30\">" << v[y][x] << "</text>\n";
      }
    }
    //Add phylogeny
    //s << get_svg_line(offset_x-5,offset_y-10+(0 * height),offset_x-5-50,offset_y-10+(0 * height), 4);
    //s << get_svg_line(offset_x-5,offset_y-10+(1 * height),offset_x-5-50,offset_y-10+(1 * height), 4);
    //s << get_svg_line(offset_x-5,offset_y-10+(2 * height),offset_x-5-50,offset_y-10+(2 * height), 4);
    //s << get_svg_line(offset_x-5,offset_y-10+(3 * height),offset_x-5-50,offset_y-10+(3 * height), 4);
    /*

    E-A
  K-F
  | G-B
N-L
  | H-C
  M-I
    J-D
    */
  }
  //Show codons
  {
    const int offset_y{250};
    const int width{30};
    const int height{30};
    const auto v = get_group_codons();
    for (int y=0; y!=4; ++y)
    {
      const int sz = v[y].size();
      for (int x=0; x!=sz; ++x)
      {
        const int x_co = offset_x + (x * width);
        const int y_co = offset_y + (y * height);
        s << "  <rect x=\""<< (x_co - 5) << "\" y=\""<< (y_co + 10 - height) << "\" width=\"" << width << "\" height=\"" << height << "\"  style=\"fill:white;stroke:black;stroke-width:2;opacity:1\" />\n";
        s << "  <text x=\""<< x_co << "\" y=\""<< y_co << "\" font-family=\"Verdana\" font-size=\"20\">" << v[y][x] << "</text>\n";
      }
    }
  }
  s << "</svg>\n";
  return s.str();
}

void create_svg()
{
  {
    std::ofstream f("tece_logo.svg");
    f << create_svg_string();
  }
  std::system("inkview tece_logo.svg");
  //std::system("display tece_logo.svg");
}

int main()
{
  const auto v = get_group_codons();
  for (const auto s:v) { std::cout << s << '\n'; }

  const auto a = v[0];
  const auto b = v[1];
  const auto c = v[2];
  const auto d = v[3];
  assert(get_distance(a,a) == 0);

  std::cout << std::boolalpha
    << (get_distance(a, b) < get_distance(a, c)) << '\n'
    << (get_distance(a, b) < get_distance(a, d)) << '\n'
    << (get_distance(a, b) < get_distance(b, c)) << '\n'
    << (get_distance(a, b) < get_distance(b, d)) << '\n'
    << (get_distance(c, d) < get_distance(c, a)) << '\n'
    << (get_distance(c, d) < get_distance(c, b)) << '\n'
    << (get_distance(c, d) < get_distance(d, a)) << '\n'
    << (get_distance(c, d) < get_distance(d, b)) << '\n'
  ;

  //Show matrix
  for (int y = 0; y!=4; ++y)
  {
    for (int x = 0; x!=4; ++x)
    {
      if (x == y)
      {
        std::cout << "-- ";
      }
      else
      {
        std::cout << get_distance(v[x], v[y]) << " ";
      }
    }
    std::cout << '\n';
  }

  std::cout << "CONCLUSION: " << is_okay(v) << '\n';

  create_svg();
}

/*

ACACACGAA   CGAGAAACAATTTGCGCATTATAC
GAAGTA   TTA   ACAATT   AACGCACGATAC
ATGATGTGC   ATGATG   AACATTACATACATG
ATGGAATGC   TTA   GGATACATGATGATGATG
true
true
true
true
true
true
true
true
-- 25 28 28
25 -- 31 33
28 31 -- 18
28 33 18 --

*/
