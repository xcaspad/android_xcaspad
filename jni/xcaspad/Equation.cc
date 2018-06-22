// -*- mode:C++ ; compile-command: "g++-3.4 -I. -I.. -g -c Equation.cc -DHAVE_CONFIG_H -DIN_GIAC -Wall" -*-
/*
 *  Copyright (C) 2005,2014 B. Parisse, Institut Fourier, 38402 St Martin d'Heres
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef IN_GIAC
#include <giac/first.h>
#else
#include "first.h"
#endif
#include <string>
#include "Equation.h"

#ifndef IN_GIAC
#include <giac/giac.h>
#else
#include "giac.h"
#endif

#include <pngwriter.h>

using namespace std;
using namespace giac;

#define FONTPHAT "/data/data/android_xcas.mucephi.necessitas.kde.org.xcaspad/DejaVuSans.ttf"

#define FONTZIZE 9

namespace xcas {

  unsigned max_prettyprint_equation = 5000;

  FT_Font cst_greek_translate(string & s0) {
    string s1(s0);
    s0 = "";
    int n = s1.size();
    for (int i = 0; i < n; ++i) {
      if (!isalpha(s1[i])) {
        s0 += s1[i];
        continue;
      }
      string s;
      for (; i < n; ++i) {
        if (!isalpha(s1[i])) {
          --i;
          break;
        }
        s += s1[i];
      }
      bool done = false;
      switch (s.size()) {
      case 2:
        if (s == "mu") {
          s0 += "μ";
          done = true;
        }
        if (s == "nu") {
          s0 += "ν";
          done = true;
        }
        if (s == "pi") {
          s0 += "π";
          done = true;
        }
        if (s == "xi") {
          s0 += "ξ";
          done = true;
        }
        if (s == "Xi") {
          s0 += "Ξ";
          done = true;
        }
        break;
      case 3:
        if (s == "chi") {
          s0 += "χ";
          done = true;
        }
        if (s == "phi") {
          s0 += "φ";
          done = true;
        }
        if (s == "Phi") {
          s0 += "Φ";
          done = true;
        }
        if (s == "eta") {
          s0 += "η";
          done = true;
        }
        if (s == "rho") {
          s0 += "ρ";
          done = true;
        }
        if (s == "tau") {
          s0 += "τ";
          done = true;
        }
        if (s == "psi") {
          s0 += "ψ";
          done = true;
        }
        if (s == "Psi") {
          s0 += "Ψ";
          done = true;
        }
        break;
      case 4:
        if (s == "beta") {
          s0 += "β";
          done = true;
        }
        if (s == "Beta") {
          s0 += "β";
          done = true;
        }
        if (s == "zeta") {
          s0 += "ζ";
          done = true;
        }
        if (s == "Zeta") {
          s0 += "ζ";
          done = true;
        }
        if (s == "alef") {
          s0 += "ℵ";
          done = true;
        }
        break;
      case 5:
        if (s == "alpha") {
          s0 += "α";
          done = true;
        }
        if (s == "delta") {
          s0 += "δ";
          done = true;
        }
        if (s == "Delta") {
          s0 += "Δ";
          done = true;
        }
        if (s == "gamma") {
          s0 += "γ";
          done = true;
        }
        if (s == "Gamma") {
          s0 += "Γ";
          done = true;
        }
        if (s == "kappa") {
          s0 += "κ";
          done = true;
        }
        if (s == "theta") {
          s0 += "θ";
          done = true;
        }
        if (s == "Theta") {
          s0 += "Θ";
          done = true;
        }
        if (s == "sigma") {
          s0 += "σ";
          done = true;
        }
        if (s == "Sigma") {
          s0 += "Σ";
          done = true;
        }
        if (s == "Omega") {
          s0 += "Ω";
          done = true;
        }
        if (s == "omega") {
          s0 += "ω";
          done = true;
        }
        break;
      case 6:
        if (s == "lambda") {
          s0 += "λ";
          done = true;
        }
        if (s == "Lambda") {
          s0 += "λ";
          done = true;
        }
        break;
      case 7:
        if (s == "epsilon") {
          s0 += "ε";
          done = true;
        }
        if (s == "product") {
          s0 = "Π";
          done = true;
        }
        break;
      case 8:
        if (s == "infinity") {
          s0 += "∞";
          done = true;
        }
        break;
      case 11:
        if (s == "euler_gamma") {
          s0 += "ϒ";
          done = true;
        }
        break;
      } // end switch
      if (!done)
        s0 += s;
    } // end for (int i=0;i<n;i++)
    return FL_HELVETICA;
  }

  bool Equation_multistring_selection(const_iterateur & it, const_iterateur & itend, bool search_active) {
    for (; it != itend; ++it) { // find first selection
      if (it-> type == _EQW) {
        if (search_active) {
          if (it->_EQWptr->active)
            break;
        } else {
          if (it->_EQWptr->selected)
            break;
        }
      }
    }
    if (it == itend)
      return false; // nothing selected
    const_iterateur it_end_sel = it + 1;
    for (; it_end_sel != itend; ++it_end_sel) { // find last selection
      if (it_end_sel->type == _EQW) {
        if (search_active) {
          if (!it_end_sel->_EQWptr->active)
            break;
        } else {
          if (!it_end_sel->_EQWptr->selected)
            break;
        }
      }
    }
    itend = it_end_sel - 1;
    return true;
  }

  eqwdata Equation_total_size(const gen & g) {
    if (g.type == _EQW)
      return *g._EQWptr;
    if (g.type != _VECT || g._VECTptr->empty())
      return eqwdata(0, 0, 0, 0, attributs(0, 0, 0), undef);
    return Equation_total_size(g._VECTptr->back());
  }

  void Equation_translate(gen & g, int deltax, int deltay) {
    if (g.type == _EQW) {
      g._EQWptr->x += deltax;
      g._EQWptr->y += deltay;
      g._EQWptr->baseline += deltay;
      return;
    }
    if (g.type != _VECT)
      setsizeerr();
    vecteur & v = * g._VECTptr;
    iterateur it = v.begin(), itend = v.end();
    for (; it != itend; ++it)
      Equation_translate( * it, deltax, deltay);
  }

  gen Equation_change_attributs(const gen & g, const attributs & newa) {
    if (g.type == _EQW) {
      gen res( * g._EQWptr);
      res._EQWptr->eqw_attributs = newa;
      return res;
    }
    if (g.type != _VECT)
      setsizeerr();
    vecteur v = * g._VECTptr;
    iterateur it = v.begin(), itend = v.end();
    for (; it != itend; ++it)
      * it = Equation_change_attributs( * it, newa);
    return gen(v, g.subtype);
  }

  vecteur Equation_subsizes(const gen & arg, const attributs & a, int windowhsize, GIAC_CONTEXT) {
    vecteur v;
    if ((arg.type == _VECT) && ((arg.subtype == _SEQ__VECT)
        // || (!ckmatrix(arg))
      )) {
      const_iterateur it = arg._VECTptr->begin(), itend = arg._VECTptr->end();
      for (; it != itend; ++it)
        v.push_back(Equation_compute_size( * it, a, windowhsize, contextptr));
    } else {
      v.push_back(Equation_compute_size(arg, a, windowhsize, contextptr));
    }
    return v;
  }

  // vertical merge with same baseline
  // for vertical merge of hp,yp at top (like ^) add fontsize to yp
  // at bottom (like lower bound of int) substract fontsize from yp
  void Equation_vertical_adjust(int hp, int yp, int & h, int & y) {
    int yf = min(y, yp);
    h = max(y + h, yp + hp) - yf;
    y = yf;
  }

  gen Equation_compute_symb_size(const gen & g, const attributs & a, int windowhsize, GIAC_CONTEXT) {
    if (g.type != _SYMB)
      return Equation_compute_size(g, a, windowhsize, contextptr);
    unary_function_ptr & u = g._SYMBptr->sommet;
    gen arg = g._SYMBptr->feuille, rootof_value;
    if (u == at_rootof && arg.type == _VECT && arg._VECTptr->size() == 2 && arg._VECTptr->front().type == _VECT && has_rootof_value(arg._VECTptr->back(), rootof_value, contextptr)) {
      return Equation_compute_symb_size(horner_rootof( * arg._VECTptr->front()._VECTptr, rootof_value, contextptr), a, windowhsize, contextptr);
    }
    if (u == at_multistring) {
      gen tmp = _multistring(arg, contextptr);
      tmp.subtype = 1;
      return Equation_compute_size(tmp, a, windowhsize, contextptr);
    }
    if (u == at_makevector) {
      vecteur v(1, arg);
      if (arg.type == _VECT)
        v = * arg._VECTptr;
      iterateur it = v.begin(), itend = v.end();
      for (; it != itend; ++it) {
        if ((it->type == _SYMB) && (it->_SYMBptr->sommet == at_makevector))
          * it = _makevector(it->_SYMBptr->feuille, contextptr);
      }
      return Equation_compute_size(v, a, windowhsize, contextptr);
    }
    if (u == at_makesuite) {
      if (arg.type == _VECT)
        return Equation_compute_size(gen( * arg._VECTptr, _SEQ__VECT), a, windowhsize, contextptr);
      else
        return Equation_compute_size(arg, a, windowhsize, contextptr);
    }
    if (u == at_sqrt)
      return Equation_compute_size(symbolic(at_pow, gen(makevecteur(arg, plus_one_half), _SEQ__VECT)), a, windowhsize, contextptr);
    if (u == at_division) {
      if (arg.type != _VECT || arg._VECTptr->size() != 2)
        return Equation_compute_size(arg, a, windowhsize, contextptr);
      gen tmp;
#ifdef SMARTPTR64
        * ((longlong * ) & tmp) = longlong(new ref_fraction(Tfraction < gen > (arg._VECTptr->front(), arg._VECTptr->back()))) << 16;
#else
        tmp.__FRACptr = new ref_fraction(Tfraction < gen > (arg._VECTptr->front(), arg._VECTptr->back()));
#endif
      tmp.type = _FRAC;
      return Equation_compute_size(tmp, a, windowhsize, contextptr);
    }
    if (u == at_prod) {
      gen n, d;
      if (rewrite_prod_inv(arg, n, d)) {
        if (n.is_symb_of_sommet(at_neg))
          return Equation_compute_size(symbolic(at_neg, Tfraction < gen > (-n, d)), a, windowhsize, contextptr);
        return Equation_compute_size(Tfraction < gen > (n, d), a, windowhsize, contextptr);
      }
    }
    if (u == at_inv) {
      if ((is_integer(arg) && is_positive(-arg, contextptr)) || (arg.is_symb_of_sommet(at_neg)))
        return Equation_compute_size(symbolic(at_neg, Tfraction < gen > (plus_one, -arg)), a, windowhsize, contextptr);
      return Equation_compute_size(Tfraction < gen > (plus_one, arg), a, windowhsize, contextptr);
    }
    if (u == at_expr && arg.type == _VECT && arg.subtype == _SEQ__VECT && arg._VECTptr->size() == 2 && arg._VECTptr->back().type == _INT_) {
      gen varg1 = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
      eqwdata vv(Equation_total_size(varg1));
      gen varg2 = eqwdata(0, 0, 0, 0, a, arg._VECTptr->back());
      vecteur v12(makevecteur(varg1, varg2));
      v12.push_back(eqwdata(vv.dx, vv.dy, 0, vv.y, a, at_expr, 0));
      return gen(v12, _SEQ__VECT);
    }
    int llp = int(check_fl_width("("));
    int lrp = int(check_fl_width(")"));
    int lc = int(check_fl_width(","));
    string us = u.ptr()->s;
    fl_font(cst_greek_translate(us), a.fontsize);
    int ls = int(check_fl_width(us.c_str()));
    fl_font(FL_HELVETICA, a.fontsize);
    if (isalpha(u.ptr()->s[0]))
      ls += 2;
    if (u == at_abs)
      ls = 2;
    // special cases first int, sigma, /, ^
    // and if printed as printsommetasoperator
    // otherwise print with usual functional notation
    int x = 0;
    int h = a.fontsize;
    int y = 0;
    if ((u == at_integrate) || (u == at_sum)) { // Int
      int s = 1;
      if (arg.type == _VECT)
        s = arg._VECTptr->size();
      else
        arg = vecteur(1, arg);
      // s==1 -> general case
      if ((s == 1) || (s == 2)) { // int f(x) dx and sum f(n) n
        vecteur v(Equation_subsizes(gen( * arg._VECTptr, _SEQ__VECT), a, windowhsize, contextptr));
        eqwdata vv(Equation_total_size(v[0]));
        if (s == 1) {
          x = a.fontsize;
          Equation_translate(v[0], x, 0);
          x += int(check_fl_width(" dx"));
        }
        if (s == 2) {
          if (u == at_integrate) {
            x = a.fontsize;
            Equation_translate(v[0], x, 0);
            x += vv.dx + int(check_fl_width(" d"));
            Equation_vertical_adjust(vv.dy, vv.y, h, y);
            vv = Equation_total_size(v[1]);
            Equation_translate(v[1], x, 0);
            Equation_vertical_adjust(vv.dy, vv.y, h, y);
          } else {
            Equation_vertical_adjust(vv.dy, vv.y, h, y);
            eqwdata v1 = Equation_total_size(v[1]);
            x = max(a.fontsize, v1.dx) + a.fontsize / 3; // var name size
            Equation_translate(v[1], 0, -v1.dy - v1.y);
            Equation_vertical_adjust(v1.dy, -v1.dy, h, y);
            Equation_translate(v[0], x, 0);
            x += vv.dx; // add function size
          }
        }
        if (u == at_integrate) {
          x += vv.dx;
          if (h == a.fontsize)
            h += 2 * a.fontsize / 3;
          if (y == 0) {
            y = -2 * a.fontsize / 3;
            h += 2 * a.fontsize / 3;
          }
        }
        v.push_back(eqwdata(x, h, 0, y, a, u, 0));
        return gen(v, _SEQ__VECT);
      }
      if (s >= 3) { // int _a^b f(x) dx
        vecteur & intarg = * arg._VECTptr;
        gen tmp_l, tmp_u, tmp_f, tmp_x;
        attributs aa(a);
        if (a.fontsize >= 10)
          aa.fontsize -= 2;
        tmp_f = Equation_compute_size(intarg[0], a, windowhsize, contextptr);
        tmp_x = Equation_compute_size(intarg[1], a, windowhsize, contextptr);
        tmp_l = Equation_compute_size(intarg[2], aa, windowhsize, contextptr);
        if (s == 4)
          tmp_u = Equation_compute_size(intarg[3], aa, windowhsize, contextptr);
        x = a.fontsize;
        eqwdata vv(Equation_total_size(tmp_l));
        Equation_translate(tmp_l, x - 2, -vv.y - vv.dy);
        vv = Equation_total_size(tmp_l);
        Equation_vertical_adjust(vv.dy, vv.y, h, y);
        int lx = vv.dx;
        if (s == 4) {
          vv = Equation_total_size(tmp_u);
          Equation_translate(tmp_u, x, a.fontsize - 3 - vv.y);
          vv = Equation_total_size(tmp_u);
          Equation_vertical_adjust(vv.dy, vv.y, h, y);
        }
        x += max(lx, vv.dx);
        Equation_translate(tmp_f, x, 0);
        vv = Equation_total_size(tmp_f);
        Equation_vertical_adjust(vv.dy, vv.y, h, y);
        if (u == at_integrate) {
          x += vv.dx + int(check_fl_width(" d"));
          Equation_translate(tmp_x, x, 0);
          vv = Equation_total_size(tmp_x);
          Equation_vertical_adjust(vv.dy, vv.y, h, y);
          x += vv.dx;
        } else {
          x += vv.dx;
          Equation_vertical_adjust(vv.dy, vv.y, h, y);
          vv = Equation_total_size(tmp_x);
          x = max(x, vv.dx) + a.fontsize / 3;
          Equation_translate(tmp_x, 0, -vv.dy - vv.y);
          Equation_vertical_adjust(vv.dy, -vv.dy, h, y);
        }
        vecteur res(makevecteur(tmp_f, tmp_x, tmp_l));
        if (s == 4)
          res.push_back(tmp_u);
        res.push_back(eqwdata(x, h, 0, y, a, u, 0));
        return gen(res, _SEQ__VECT);
      }
    }
    if (u == at_limit && arg.type == _VECT) { // limit
      vecteur & limarg = * arg._VECTptr;
      int s = limarg.size();
      if (s >= 3) {
        gen tmp_l, tmp_f, tmp_x, tmp_dir;
        attributs aa(a);
        if (a.fontsize >= 10)
          aa.fontsize -= 2;
        tmp_f = Equation_compute_size(limarg[0], a, windowhsize, contextptr);
        tmp_x = Equation_compute_size(limarg[1], aa, windowhsize, contextptr);
        tmp_l = Equation_compute_size(limarg[2], aa, windowhsize, contextptr);
        if (s == 4)
          tmp_dir = Equation_compute_size(limarg[3], aa, windowhsize, contextptr);
        eqwdata vf(Equation_total_size(tmp_f));
        eqwdata vx(Equation_total_size(tmp_x));
        eqwdata vl(Equation_total_size(tmp_l));
        eqwdata vdir(Equation_total_size(tmp_dir));
        int sous = max(vx.dy, vl.dy);
        if (s == 4)
          Equation_translate(tmp_f, vx.dx + vl.dx + vdir.dx + a.fontsize + 4, 0);
        else
          Equation_translate(tmp_f, vx.dx + vl.dx + a.fontsize + 2, 0);
        Equation_translate(tmp_x, 0, -sous - vl.y);
        Equation_translate(tmp_l, vx.dx + a.fontsize + 2, -sous - vl.y);
        if (s == 4)
          Equation_translate(tmp_dir, vx.dx + vl.dx + a.fontsize + 4, -sous - vl.y);
        h = vf.dy;
        y = vf.y;
        vl = Equation_total_size(tmp_l);
        Equation_vertical_adjust(vl.dy, vl.y, h, y);
        vecteur res(makevecteur(tmp_f, tmp_x, tmp_l));
        if (s == 4) {
          res.push_back(tmp_dir);
          res.push_back(eqwdata(vf.dx + vx.dx + a.fontsize + 4 + vl.dx + vdir.dx, h, 0, y, a, u, 0));
        } else
          res.push_back(eqwdata(vf.dx + vx.dx + a.fontsize + 2 + vl.dx, h, 0, y, a, u, 0));
        return gen(res, _SEQ__VECT);
      }
    }
    if ((u == at_of || u == at_at) && arg.type == _VECT && arg._VECTptr->size() == 2) {
      // user function, function in 1st arg, arguments in 2nd arg
      gen varg1 = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
      eqwdata vv = Equation_total_size(varg1);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      gen arg2 = arg._VECTptr->back();
      if (u == at_at && xcas_mode(contextptr) != 0) {
        if (arg2.type == _VECT)
          arg2 = gen(addvecteur( * arg2._VECTptr, vecteur(arg2._VECTptr->size(), plus_one)), _SEQ__VECT);
        else
          arg2 = arg2 + plus_one;
      }
      gen varg2 = Equation_compute_size(arg2, a, windowhsize, contextptr);
      Equation_translate(varg2, vv.dx + llp, 0);
      vv = Equation_total_size(varg2);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      vecteur res(makevecteur(varg1, varg2));
      res.push_back(eqwdata(vv.dx + vv.x + lrp, h, 0, y, a, u, 0));
      return gen(res, _SEQ__VECT);
    }
    if (u == at_pow) {
      // first arg not translated
      gen varg = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
      eqwdata vv = Equation_total_size(varg);
      // 1/2 ->sqrt, otherwise as exponent
      if (arg._VECTptr->back() == plus_one_half) {
        Equation_translate(varg, a.fontsize, 0);
        vecteur res(1, varg);
        res.push_back(eqwdata(vv.dx + a.fontsize, vv.dy + 4, vv.x, vv.y, a, at_sqrt, 0));
        return gen(res, _SEQ__VECT);
      }
      if (vv.g.type == _FUNC || vv.g.is_symb_of_sommet(at_pow))
        x = llp;
      Equation_translate(varg, x, 0);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      vecteur res(1, varg);
      // 2nd arg translated
      if (vv.g.type == _FUNC || vv.g.is_symb_of_sommet(at_pow))
        x += vv.dx + lrp;
      else
        x += vv.dx + 1;
      int arg1dy = vv.dy, arg1y = vv.y;
      if (a.fontsize >= 10) {
        attributs aa(a);
        aa.fontsize -= 2;
        varg = Equation_compute_size(arg._VECTptr->back(), aa, windowhsize, contextptr);
      } else
        varg = Equation_compute_size(arg._VECTptr->back(), a, windowhsize, contextptr);
      vv = Equation_total_size(varg);
      Equation_translate(varg, x, arg1y + (3 * arg1dy) / 4 - vv.y);
      res.push_back(varg);
      vv = Equation_total_size(varg);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      x += vv.dx;
      res.push_back(eqwdata(x, h, 0, y, a, u, 0));
      return gen(res, _SEQ__VECT);
    }
    if (u == at_factorial) {
      vecteur v;
      gen varg = Equation_compute_size(arg, a, windowhsize, contextptr);
      eqwdata vv = Equation_total_size(varg);
      bool paren = need_parenthesis(vv.g) || vv.g == at_prod || vv.g == at_division || vv.g == at_pow;
      if (paren)
        x += llp;
      Equation_translate(varg, x, 0);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      v.push_back(varg);
      x += vv.dx;
      if (paren)
        x += lrp;
      varg = eqwdata(x + 4, h, 0, y, a, u, 0);
      v.push_back(varg);
      return gen(v, _SEQ__VECT);
    }
    if (u == at_sto) { // A:=B, *it -> B
      gen varg = Equation_compute_size(arg._VECTptr->back(), a, windowhsize, contextptr);
      eqwdata vv = Equation_total_size(varg);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      Equation_translate(varg, x, 0);
      vecteur v(2);
      v[1] = varg;
      x += vv.dx;
      x += ls + 3;
      // first arg not translated
      varg = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
      vv = Equation_total_size(varg);
      if (need_parenthesis(vv.g))
        x += llp;
      Equation_translate(varg, x, 0);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      v[0] = varg;
      x += vv.dx;
      if (need_parenthesis(vv.g))
        x += lrp;
      v.push_back(eqwdata(x, h, 0, y, a, u, 0));
      return gen(v, _SEQ__VECT);
    }
    if (u == at_program && arg._VECTptr->back().type != _VECT && !arg._VECTptr->back().is_symb_of_sommet(at_local)) {
      gen varg = Equation_compute_size(arg._VECTptr->front(), a, windowhsize, contextptr);
      eqwdata vv = Equation_total_size(varg);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      Equation_translate(varg, x, 0);
      vecteur v(2);
      v[0] = varg;
      x += vv.dx;
      x += int(check_fl_width("->")) + 3;
      varg = Equation_compute_size(arg._VECTptr->back(), a, windowhsize, contextptr);
      vv = Equation_total_size(varg);
      Equation_translate(varg, x, 0);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      v[1] = varg;
      x += vv.dx;
      v.push_back(eqwdata(x, h, 0, y, a, u, 0));
      return gen(v, _SEQ__VECT);
    }
    bool binaryop = (u.ptr()->printsommet == & printsommetasoperator) || equalposcomp(binary_op_tab(), u);
    if (u != at_sto && u.ptr()->printsommet != NULL && !binaryop) {
      gen tmp = string2gen(g.print(contextptr), false);
      return Equation_compute_size(symbolic(at_expr, gen(makevecteur(tmp, xcas_mode(contextptr)), _SEQ__VECT)), a, windowhsize, contextptr);
    }
    vecteur v;
    if (!binaryop || arg.type != _VECT)
      v = Equation_subsizes(arg, a, windowhsize, contextptr);
    else
      v = Equation_subsizes(gen( * arg._VECTptr, _SEQ__VECT), a, windowhsize, contextptr);
    iterateur it = v.begin(), itend = v.end();
    if (it == itend || (itend - it == 1)) {
      gen gtmp;
      if (it == itend)
        gtmp = Equation_compute_size(gen(vecteur(0), _SEQ__VECT), a, windowhsize, contextptr);
      else
        gtmp = * it;
      // unary op, shift arg position horizontally
      eqwdata vv = Equation_total_size(gtmp);
      bool paren = u != at_neg || (vv.g != at_prod && need_parenthesis(vv.g));
      x = ls + (paren ? llp : 0);
      gen tmp = gtmp;
      Equation_translate(tmp, x, 0);
      x = x + vv.dx + (paren ? lrp : 0);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      return gen(makevecteur(tmp, eqwdata(x, h, 0, y, a, u, 0)), _EQW__VECT);
    }
    if (binaryop) { // op (default with par)
      int currenth = h, largeur = 0;
      iterateur itprec = v.begin();
      h = 0;
      if (u == at_plus) { // op without parenthesis
        llp = 0;
        lrp = 0;
      }
      for (;;) {
        eqwdata vv = Equation_total_size( * it);
        if (need_parenthesis(vv.g))
          x += llp;
        if (u == at_plus && it != v.begin() &&
          (
            (it->type == _VECT && it->_VECTptr->back().type == _EQW && it->_VECTptr->back()._EQWptr->g == at_neg) ||
            (it->type == _EQW && (is_integer(it->_EQWptr->g) || it->_EQWptr->g.type == _DOUBLE_) && is_strictly_positive(-it->_EQWptr->g, contextptr))
          )
        )
          x -= ls;
        if (x > windowhsize - vv.dx && x > windowhsize / 2 && (itend - it) * vv.dx > windowhsize / 2) {
          largeur = max(x, largeur);
          x = 0;
          if (need_parenthesis(vv.g))
            x += llp;
          h += currenth;
          Equation_translate( * it, x, 0);
          for (iterateur kt = v.begin(); kt != itprec; ++kt)
            Equation_translate( * kt, 0, currenth);
          if (y) {
            for (iterateur kt = itprec; kt != it; ++kt)
              Equation_translate( * kt, 0, -y);
          }
          itprec = it;
          currenth = vv.dy;
          y = vv.y;
        } else {
          Equation_translate( * it, x, 0);
          vv = Equation_total_size( * it);
          Equation_vertical_adjust(vv.dy, vv.y, currenth, y);
        }
        x += vv.dx;
        if (need_parenthesis(vv.g))
          x += lrp;
        ++it;
        if (it == itend) {
          for (iterateur kt = v.begin(); kt != itprec; ++kt)
            Equation_translate( * kt, 0, currenth + y);
          h += currenth;
          v.push_back(eqwdata(max(x, largeur), h, 0, y, a, u, 0));
          // cerr << v << endl;
          return gen(v, _SEQ__VECT);
        }
        x += ls + 3;
      }
    }
    // normal printing
    x = ls + llp;
    for (;;) {
      eqwdata vv = Equation_total_size( * it);
      Equation_translate( * it, x, 0);
      Equation_vertical_adjust(vv.dy, vv.y, h, y);
      x += vv.dx;
      ++it;
      if (it == itend) {
        x += lrp;
        v.push_back(eqwdata(x, h, 0, y, a, u, 0));
        return gen(v, _SEQ__VECT);
      }
      x += lc;
    }
  }

  // windowhsize is used for g of type HIST_EQW (history) right justify answers
  // Returns either a eqwdata type object (terminal) or a vector
  // (of subtype _EQW__VECT or _HIST__VECT)
  gen Equation_compute_size(const gen & g, const attributs & a, int windowhsize, GIAC_CONTEXT) {
    fl_font(FL_HELVETICA, a.fontsize);
    /***************
     *   STRINGS   *
     ***************/
    if (g.type == _STRNG) {
      string s;
      s = * g._STRNGptr;
      string cs;
      int ss = s.size();
      /* if (!ss)
     return eqwdata(10,6,0,0,a,g); */
      int hsize = 0, vsize = 0;
      bool newline = false;
      vecteur res;
      gen tmps;
      for (int pos = 0; pos < ss; ++pos) {
        char ch = s[pos];
        if (ch == '\n') {
          newline = true;
          hsize = max(hsize, int(check_fl_width((' ' + cs).c_str())));
          tmps = string2gen(cs, false);
          vsize += a.fontsize;
          res.push_back(eqwdata(hsize, a.fontsize, 0, -vsize, a, tmps));
          cs = "";
        } else
          cs += ch;
      }
      hsize = max(hsize, int(check_fl_width((' ' + cs).c_str())));
      vsize += a.fontsize;
      tmps = string2gen(cs, false);
      if (!newline) {
        tmps.subtype = g.subtype;
        return eqwdata(hsize, a.fontsize, 0, 0, a, tmps);
      }
      gen tmp = eqwdata(hsize, a.fontsize, 0, -vsize, a, tmps);
      res.push_back(tmp);
      res.push_back(eqwdata(hsize, vsize, 0, -vsize, a, at_multistring));
      tmp = gen(res, _EQW__VECT);
      Equation_translate(tmp, 0, vsize);
      return tmp;
    }
    /*****************
     *   FRACTIONS   *
     *****************/
    if (g.type == _FRAC) {
      if (is_integer(g._FRACptr->num) && is_positive(-g._FRACptr->num, contextptr))
        return Equation_compute_size(symbolic(at_neg, fraction(-g._FRACptr->num, g._FRACptr->den)), a, windowhsize, contextptr);
      gen v1 = Equation_compute_size(g._FRACptr->num, a, windowhsize, contextptr);
      eqwdata vv1 = Equation_total_size(v1);
      gen v2 = Equation_compute_size(g._FRACptr->den, a, windowhsize, contextptr);
      eqwdata vv2 = Equation_total_size(v2);
      // Center the fraction
      int w1 = vv1.dx, w2 = vv2.dx;
      int w = max(w1, w2) + 6;
      vecteur v(3);
      v[0] = v1;
      Equation_translate(v[0], (w - w1) / 2, 11 - vv1.y);
      v[1] = v2;
      Equation_translate(v[1], (w - w2) / 2, 7 - vv2.dy - vv2.y);
      v[2] = eqwdata(w, 4 + vv1.dy + vv2.dy, 0, 7 - vv2.dy, a, at_division, 0);
      return gen(v, _SEQ__VECT);
    }
    /***************
     *   VECTORS   *
     ***************/
    if ((g.type == _VECT) && !g._VECTptr->empty()) {
      if (g.subtype == _SPREAD__VECT)
        return Equation_compute_size(string2gen("spreadsheet", false), a, windowhsize, contextptr);
      vecteur v;
      const_iterateur it = g._VECTptr->begin(), itend = g._VECTptr->end();
      int x = 0, y = 0, h = a.fontsize;
      /***************
       *   HISTORY   *
       ***************/
      if (g.subtype == _HIST__VECT) {
        int l = windowhsize;
        vecteur vplot;
        for (int i = 0; it != itend; ++it, ++i) {
          gen tmpg( * it);
          if (!rpn_mode(contextptr) && it->type == _VECT && !it->_VECTptr->empty()) {
            if (it->_VECTptr->front().type == _STRNG)
              tmpg = makevecteur(it->_VECTptr->front(), string2gen("", false));
            gen tmpback = it->_VECTptr->back();
            if (tmpback.type == _POINTER_ && tmpback.subtype == _FL_WIDGET_POINTER && fl_widget_updatepict_function)
              tmpback = fl_widget_updatepict_function(tmpback);
            if (tmpback.is_symb_of_sommet(at_pnt) || (tmpback.type == _VECT && !tmpback._VECTptr->empty() && tmpback._VECTptr->back().is_symb_of_sommet(at_pnt)))
              vplot.push_back(tmpback);
            if (tmpback.is_symb_of_sommet(at_erase))
              vplot.clear();
            gen itfront = it->_VECTptr->front();
            if (itfront.is_symb_of_sommet(at_expr)) {
              itfront = itfront._SYMBptr->feuille;
              int mode = xcas_mode(contextptr);
              if (itfront.type == _VECT && !itfront._VECTptr->empty()) {
                mode = itfront._VECTptr->back().val;
                itfront = itfront._VECTptr->front();
              }
              if (itfront.type == _STRNG) {
                int save_maple_mode = xcas_mode(contextptr);
                xcas_mode(contextptr) = mode;
                try {
                  itfront = gen( * itfront._STRNGptr, contextptr);
                } catch (std::runtime_error & e) {}
                xcas_mode(contextptr) = save_maple_mode;
              }
            }
          }
          tmpg.subtype = _SEQ__VECT;
          vecteur tmp(Equation_subsizes(tmpg, a, windowhsize, contextptr));
          iterateur jt = tmp.begin(); // this is the question
          // compute the size of writing the history level i
          eqwdata w(Equation_total_size( * jt));
          if (rpn_mode(contextptr)) // ignore question
            v.push_back(eqwdata(1, 1, x, -y, w.eqw_attributs, string2gen("", false)));
          else {
            y += w.dy + 2;
            x = int(check_fl_width((print_INT_(i) + ": ").c_str()));
            if (l < w.dx + x)
              l = w.dx + x;
            Equation_translate( * jt, x, -y - w.y);
            v.push_back( * jt);
          }
          jt = tmp.end() - 1; // this is the answer
          * jt = Equation_change_attributs( * jt, attributs(a.fontsize, a.background, a.text_color + 4));
          w = Equation_total_size( * jt);
          y += w.dy + 2;
          x = int(check_fl_width("    "));
          l = max(l, w.dx + x);
          int xshift = x;
          if (w.dx + 4 < windowhsize - a.fontsize) {
            if (center_history)
              xshift = (windowhsize - w.dx - 4) / 2;
            else
              xshift = windowhsize - w.dx - 4 - a.fontsize;
          }
          Equation_translate( * jt, xshift, -y - w.y);
          v.push_back( * jt);
        }
        v.push_back(eqwdata(l, y, 0, -y, a, at_makevector, 0));
        // cerr << v << endl;
        gen res = gen(v, _HIST__VECT);
        Equation_translate(res, 0, y);
        return res;
      } // END HISTORY
      /***************
       *   MATRICE   *
       ***************/
      if (ckmatrix(g) && g.subtype != _SEQ__VECT && g.subtype != _SET__VECT && g._VECTptr->front().subtype != _SEQ__VECT) {
        gen mkvect(at_makevector);
        mkvect.subtype = _SEQ__VECT;
        gen mkmat(at_makevector);
        mkmat.subtype = _MATRIX__VECT;
        int nrows, ncols;
        mdims( * g._VECTptr, nrows, ncols);
        if (ncols) {
          vecteur all_sizes;
          all_sizes.reserve(nrows);
          vector < int > row_heights(nrows), row_bases(nrows), col_widths(ncols);
          // vertical gluing
          for (int i = 0; it != itend; ++it, ++i) {
            gen tmpg = * it;
            tmpg.subtype = _SEQ__VECT;
            vecteur tmp(Equation_subsizes(tmpg, a, max(windowhsize / ncols - a.fontsize, 230), contextptr));
            int h = a.fontsize, y = 0;
            const_iterateur jt = tmp.begin(), jtend = tmp.end();
            for (int j = 0; jt != jtend; ++jt, ++j) {
              eqwdata w(Equation_total_size( * jt));
              Equation_vertical_adjust(w.dy, w.y, h, y);
              col_widths[j] = max(col_widths[j], w.dx);
            }
            if (i)
              row_heights[i] = row_heights[i - 1] + h + a.fontsize;
            else
              row_heights[i] = h;
            row_bases[i] = y;
            all_sizes.push_back(tmp);
          }
          // accumulate col widths
          col_widths.front() += (3 * a.fontsize) / 2;
          vector < int > ::iterator iit = col_widths.begin() + 1, iitend = col_widths.end();
          for (; iit != iitend; ++iit)
            * iit += * (iit - 1) + a.fontsize;
          // translate each cell
          it = all_sizes.begin();
          itend = all_sizes.end();
          int h, y, prev_h = 0;
          for (int i = 0; it != itend; ++it, ++i) {
            h = row_heights[i];
            y = row_bases[i];
            iterateur jt = it->_VECTptr->begin(), jtend = it->_VECTptr->end();
            for (int j = 0; jt != jtend; ++jt, ++j) {
              eqwdata w(Equation_total_size( * jt));
              if (j)
                Equation_translate( * jt, col_widths[j - 1] - w.x, -h - y);
              else
                Equation_translate( * jt, -w.x + a.fontsize / 2, -h - y);
            }
            it->_VECTptr->push_back(eqwdata(col_widths.back(), h - prev_h, 0, -h, a, mkvect, 0));
            prev_h = h;
          }
          all_sizes.push_back(eqwdata(col_widths.back(), row_heights.back(), 0, -row_heights.back(), a, mkmat, -row_heights.back() / 2));
          gen all_sizesg = all_sizes;
          Equation_translate(all_sizesg, 0, row_heights.back() / 2);
          return all_sizesg;
        }
      } // end matrices
      /*************************
       *   SEQUENCES/VECTORS   *
       *************************/
      // horizontal gluing
      x += a.fontsize / 2;
      int ncols = itend - it;
      //ncols=min(ncols,5);
      for (; it != itend; ++it) {
        gen cur_size = Equation_compute_size( * it, a, max(windowhsize / ncols - a.fontsize, 480), contextptr);
        eqwdata tmp = Equation_total_size(cur_size);
        Equation_translate(cur_size, x - tmp.x, 0);
        v.push_back(cur_size);
        x = x + tmp.dx + a.fontsize;
        Equation_vertical_adjust(tmp.dy, tmp.y, h, y);
      }
      gen mkvect(at_makevector);
      if (g.subtype == _SEQ__VECT)
        mkvect = at_makesuite;
      else
        mkvect.subtype = g.subtype;
      v.push_back(eqwdata(x, h, 0, y, a, mkvect, 0));
      return gen(v, _EQW__VECT);
    } // end sequences
    if (g.type == _USER) {
      if (dynamic_cast < giac::galois_field * > (g._USERptr)) {
        gen g1(g.print(contextptr), contextptr);
        return Equation_compute_size(g1, a, windowhsize, contextptr);
      }
    }
    if (g.type != _SYMB) {
      string s = g.print(contextptr);
      if (s.size() > 2000)
        s = s.substr(0, 2000) + "...";
      fl_font(cst_greek_translate(s), a.fontsize);
      int i = int(check_fl_width(s.c_str()));
      gen tmp = eqwdata(i, a.fontsize, 0, 0, a, g);
      return tmp;
    }
    /**********************
     *  SYMBOLIC HANDLING *
     **********************/
    return Equation_compute_symb_size(g, a, windowhsize, contextptr);
    // return Equation_compute_symb_size(aplatir_fois_plus(g),a,windowhsize,contextptr);
    // aplatir_fois_plus is a problem for Equation_replace_selection
    // because it will modify the structure of the data
  }

  void Equation_draw(const eqwdata & e, int x, int y, int rightx, int lowery, Equation * equat, int begin_sel, int end_sel) {
    if ((e.dx + e.x < x) || (e.x > rightx) || (e.y > y) || e.y + e.dy < lowery)
      return; // nothing to draw, out of window
    const giac::context * contextptr = 0; //get_context(eq);
    gen gg = e.g;
    bool selected = false; //e.selected && Fl::focus()==eq;
    int fontsize = e.eqw_attributs.fontsize;
    FT_Color text_color = FT_Color(e.eqw_attributs.text_color);
    FT_Color background = FT_Color(e.eqw_attributs.background);
    fl_font(FL_HELVETICA, fontsize);
    fl_color(selected ? background : text_color);
    if (gg.type == _POINTER_) {
      // wg->resize(e.x-x,y-e.y-e.dy,e.dx,e.dy);
      // wg->draw(); // automatically done if it belongs to the group
      return;
    }
    if (gg.type == _STRNG) {
      string s;
      if (e.active) {
        // draw s and the cursor
        s = * gg._STRNGptr;
        int ss = s.size();
        int pos = max(min(equat->active_pos, ss), 0);
        if (equat->need_active_parse)
          s = s.substr(0, pos) + "|" + s.substr(pos, ss - pos);
        else
          s = '"' + s.substr(0, pos) + "|" + s.substr(pos, ss - pos);
      } else {
        if (gg.subtype)
          s = '"' + * gg._STRNGptr;
        else
          s = ' ' + * gg._STRNGptr;
      }
      string cs;
      int ss = s.size();
      int vsize = fontsize - 2, pos;
      for (pos = 0; pos < ss; ++pos) {
        char ch = s[pos];
        if (ch == '\n') {
          check_fl_draw(cs.c_str(), equat->x() + e.x - x, equat->y() + y - e.y + vsize - e.dy);
          cs = "";
          vsize += fontsize;
        } else
          cs += ch;
      }
      check_fl_draw(cs.c_str(), equat->x() + e.x - x, equat->y() + y - e.y + vsize - e.dy);
      // If selected take care of begin/end selection
      int css = cs.size(); // must be >=1 since selected hence not active
      if (selected && css) {
        int sel0 = min(begin_sel + 1, css - 1), sel1 = css;
        if (end_sel >= 0)
          sel1 = min(end_sel + 1, css);
        if (sel0 > sel1)
          giac::swapint(sel0, sel1);
        int deltax = int(check_fl_width(cs.substr(0, sel0).c_str()));
        cs = cs.substr(sel0, sel1 - sel0);
        int dx = int(check_fl_width(cs.c_str()));
        fl_color(text_color);
        check_fl_rectf(equat->x() + e.x - x + deltax, equat->y() + y - e.y - e.dy + 1, dx, e.dy + 3);
        fl_color(background);
        check_fl_draw(cs.c_str(), equat->x() + e.x - x + deltax, equat->y() + y - e.y + vsize - e.dy);
      }
      return;
    }
    if (selected) {
      fl_color(text_color);
      check_fl_rectf(equat->x() + e.x - x, equat->y() + y - e.y - e.dy + 1, e.dx, e.dy + 3);
      fl_color(background);
    }
    string s = gg.print(contextptr);
    if (gg.type == _IDNT && !s.empty() && s[0] == '_')
      s = s.substr(1, s.size() - 1);
    if (s.size() > 2000)
      s = s.substr(0, 2000) + "...";
    FT_Font font = cst_greek_translate(s);
    if (gg.type == _IDNT && font == FL_HELVETICA
#if !defined(WIN32) && !defined(__APPLE__)
    && gg != cst_pi
# endif
    )
      font = FL_TIMES_BOLD_ITALIC; // FL_HELVETICA_BOLD_ITALIC;
    fl_font(font, fontsize);

    check_fl_draw(s.c_str(), equat->x() + e.x - x, equat->y() + y - e.y);
    return;
  }

  void Equation_draw(const gen & g, int x, int y, int rightx, int lowery, Equation * equat) {

    const giac::context * contextptr = 0;

    int eqx = equat->x(), eqy = equat->y();

    if (g.type == _EQW) { // terminal
      eqwdata & e = * g._EQWptr;
      Equation_draw(e, x, y, rightx, lowery, equat, equat->begin_sel, equat->end_sel);
    }
    if (g.type != _VECT)
      return;
    vecteur & v = * g._VECTptr;
    if (v.empty())
      return;
    gen tmp = v.back();
    if (tmp.type != _EQW) {
      cerr << "EQW error:" << v << endl;
      return;
    }
    eqwdata & w = * tmp._EQWptr;
    if ((w.dx + w.x - x < 0) || (w.x > rightx) || (w.y > y) || (w.y + w.dy < lowery))
      return; // nothing to draw, out of window
    /*******************
     * draw the vector *
     *******************/
    // v is the vector, w the master operator eqwdata
    gen oper = w.g;
    bool selected = false; //w.selected && Fl::focus()==equat;
    int fontsize = w.eqw_attributs.fontsize;
    int background = w.eqw_attributs.background;
    int text_color = w.eqw_attributs.text_color;
    int x0 = w.x;
    int y0 = w.y; // lower coordinate of the master vector
    int y1 = y0 + w.dy; // upper coordinate of the master vector
    if (selected) {
      fl_color(text_color);
      check_fl_rectf(eqx + w.x - x, eqy + y - w.y - w.dy + 1, w.dx, w.dy + 3);
    } else {
      fl_color(background);
      // fl_rectf(w.x-x,y-w.y-w.dy,w.dx,w.dy-2);
    }
    // draw arguments of v
    const_iterateur it = v.begin(), itend = v.end() - 1;
    if (oper == at_multistring) {
      const_iterateur it_beg = it, it_end = itend;
      if (Equation_multistring_selection(it_beg, it_end, false)) {
        // begin_sel and end_sel apply respect. to it_beg and it_end sel. lines
        for (; it != it_beg; ++it)
          Equation_draw( * it->_EQWptr, x, y, rightx, lowery, equat, -1, -1);
        if (it_beg == it_end) {
          Equation_draw( * it->_EQWptr, x, y, rightx, lowery, equat, equat->begin_sel, equat->end_sel);
          ++it;
        } else {
          Equation_draw( * it->_EQWptr, x, y, rightx, lowery, equat, equat->begin_sel, -1);
          ++it;
          for (; it != it_end; ++it)
            Equation_draw( * it->_EQWptr, x, y, rightx, lowery, equat, -1, -1);
          Equation_draw( * it->_EQWptr, x, y, rightx, lowery, equat, -1, equat->end_sel);
          ++it;
        }
        for (; it != itend; ++it)
          Equation_draw( * it->_EQWptr, x, y, rightx, lowery, equat, -1, -1);
        return;
      }
    }
    if (oper == at_expr && v.size() == 3) {
      Equation_draw( * it, x, y, rightx, lowery, equat);
      return;
    }
    for (; it != itend; ++it)
      Equation_draw( * it, x, y, rightx, lowery, equat);
    if (oper == at_multistring)
      return;
    fl_font(FL_HELVETICA, fontsize);
    fl_color(selected ? background : text_color);
    string s;
    if (g.subtype == _HIST__VECT) { // For history, we must write history levels
      it = v.begin();
      int nlevels = (itend - it) / 2 - 1, wlevel;
      int skip = 2; // int skip=2-rpn_mode(contextptr);
      for (int i = 0; it != itend; it += skip, ++i) {
        eqwdata tmp = Equation_total_size( * it);
        fl_font(FL_HELVETICA, tmp.eqw_attributs.fontsize);
        fl_color(FL_BLUE);
        // cerr << tmp << endl;
        int yy;
        // uncommented, seemed previously to be problematic with strings
        if (tmp.hasbaseline)
          yy = y - tmp.baseline;
        else
          yy = y - tmp.y - (tmp.dy - tmp.eqw_attributs.fontsize) / 2;
        if (yy < 0 || yy > y - lowery)
          continue;
        if (rpn_mode(contextptr))
          wlevel = nlevels - i;
        else
          wlevel = i;
        if (wlevel || !rpn_mode(contextptr))
          check_fl_draw((print_INT_(wlevel) + ": ").c_str(), eqx - x, eqy + yy);
      }
      return; // nothing else to do
    }
    if (oper.type == _FUNC) {
      // catch here special cases user function, vect/matr, ^, int, sqrt, etc.
      unary_function_ptr & u = * oper._FUNCptr;
      if (u == at_at) { // draw brackets around 2nd arg
        gen arg2 = v[1]; // 2nd arg of at_of, i.e. what's inside the parenth.
        eqwdata varg2 = Equation_total_size(arg2);
        x0 = varg2.x;
        y0 = varg2.y;
        y1 = y0 + varg2.dy;
        fontsize = varg2.eqw_attributs.fontsize;
        fl_font(FL_HELVETICA, fontsize);
        if (x0 < rightx)
          check_fl_draw("[", eqx + x0 - x - int(check_fl_width("[")), eqy + y - varg2.baseline);
        x0 += varg2.dx;
        if (x0 < rightx)
          check_fl_draw("]", eqx + x0 - x, eqy + y - varg2.baseline);
        return;
      }
      if (u == at_of) { // do we need to draw some parenthesis?
        gen arg2 = v[1]; // 2nd arg of at_of, i.e. what's inside the parenth.
        if (arg2.type != _VECT || arg2._VECTptr->back().type != _EQW || arg2._VECTptr->back()._EQWptr->g != at_makesuite) { // Yes (if not _EQW it's a sequence with parent)
          eqwdata varg2 = Equation_total_size(arg2);
          x0 = varg2.x;
          y0 = varg2.y;
          y1 = y0 + varg2.dy;
          fontsize = varg2.eqw_attributs.fontsize;
          int pfontsize = max(fontsize, (fontsize + (varg2.baseline - varg2.y)) / 2);
          fl_font(FL_HELVETICA, pfontsize); // was fontsize
          if (x0 < rightx)
            check_fl_draw("(", eqx + x0 - x - int(check_fl_width("(")), eqy + y - varg2.baseline);
          x0 += varg2.dx;
          if (x0 < rightx)
            check_fl_draw(")", eqx + x0 - x, eqy + y - varg2.baseline);
        }
        return;
      }
      if (u == at_makesuite) {
        bool paren = v.size() != 2; // Sequences with 1 arg don't show parenthesis
        int pfontsize = max(fontsize, (fontsize + (w.baseline - w.y)) / 2);
        fl_font(FL_HELVETICA, pfontsize);
        if (paren && x0 < rightx)
          check_fl_draw("(", eqx + x0 - x - int(check_fl_width("(")) / 2, eqy + y - w.baseline);
        x0 += w.dx;
        if (paren && x0 < rightx)
          check_fl_draw(")", eqx + x0 - x - int(check_fl_width("(")) / 2, eqy + y - w.baseline);
        // print commas between args
        it = v.begin(), itend = v.end() - 2;
        for (; it != itend; ++it) {
          eqwdata varg2 = Equation_total_size( * it);
          fontsize = varg2.eqw_attributs.fontsize;
          fl_font(FL_HELVETICA, fontsize);
          if (varg2.x + varg2.dx < rightx)
            check_fl_draw(",", eqx + varg2.x + varg2.dx - x + 1, eqy + y - varg2.baseline);
        }
        return;
      }
      if (u == at_makevector) { // draw [] delimiters for vector/matrices
        if (oper.subtype != _SEQ__VECT) {
          int decal = 1;
          switch (oper.subtype) {
          case _MATRIX__VECT:
            decal = 2;
            break;
          case _SET__VECT:
            decal = 4;
            break;
          case _POLY1__VECT:
            decal = 6;
            break;
          }
          if (x0 + 1 < rightx) {
            check_fl_line(eqx + x0 - x + 1, eqy + y - y0 + 1, eqx + x0 - x + 1, eqy + y - y1 + 1);
            check_fl_line(eqx + x0 - x + decal, eqy + y - y0 + 1, eqx + x0 - x + decal, eqy + y - y1 + 1);
            check_fl_line(eqx + x0 - x + 1, eqy + y - y0 + 1, eqx + x0 - x + fontsize / 4, eqy + y - y0 + 1);
            check_fl_line(eqx + x0 - x + 1, eqy + y - y1 + 1, eqx + x0 - x + fontsize / 4, eqy + y - y1 + 1);
          }
          x0 += w.dx;
          if (x0 - 1 < rightx) {
            check_fl_line(eqx + x0 - x - 1, eqy + y - y0 + 1, eqx + x0 - x - 1, eqy + y - y1 + 1);
            check_fl_line(eqx + x0 - x - decal, eqy + y - y0 + 1, eqx + x0 - x - decal, eqy + y - y1 + 1);
            check_fl_line(eqx + x0 - x - 1, eqy + y - y0 + 1, eqx + x0 - x - fontsize / 4, eqy + y - y0 + 1);
            check_fl_line(eqx + x0 - x - 1, eqy + y - y1 + 1, eqx + x0 - x - fontsize / 4, eqy + y - y1 + 1);
          }
        } // end if oper.subtype!=SEQ__VECT
        if (oper.subtype != _MATRIX__VECT) {
          // print commas between args
          it = v.begin(), itend = v.end() - 2;
          for (; it != itend; ++it) {
            eqwdata varg2 = Equation_total_size( * it);
            fontsize = varg2.eqw_attributs.fontsize;
            fl_font(FL_HELVETICA, fontsize);
            if (varg2.x + varg2.dx < rightx)
              check_fl_draw(",", eqx + varg2.x + varg2.dx - x + 1, eqy + y - varg2.baseline);
          }
        }
        return;
      }
      int lpsize = int(check_fl_width("("));
      int rpsize = int(check_fl_width(")"));
      eqwdata tmp = Equation_total_size(v.front()); // tmp= 1st arg eqwdata
      if (u == at_sto)
        tmp = Equation_total_size(v[1]);
      x0 = w.x - x;
      y0 = y - w.baseline;
      if (u == at_pow) {
        if (!need_parenthesis(tmp.g) && tmp.g != at_pow && tmp.g != at_prod && tmp.g != at_division)
          return;
        if (tmp.g == at_pow) {
          fontsize = tmp.eqw_attributs.fontsize + 2;
          fl_font(FL_HELVETICA, fontsize);
        }
        if (tmp.x - lpsize < rightx)
          check_fl_draw("(", eqx + tmp.x - x - lpsize, eqy + y - tmp.baseline);
        if (tmp.x + tmp.dx < rightx)
          check_fl_draw(")", eqx + tmp.x + tmp.dx - x, eqy + y - tmp.baseline);
        return;
      }
      if (u == at_program) {
        if (tmp.x + tmp.dx < rightx)
          check_fl_draw("->", eqx + tmp.x + tmp.dx - x, eqy + y - tmp.baseline);
        return;
      }
      if (u == at_sum) {
        if (x0 < rightx) {
          check_fl_line(eqx + x0, eqy + y0, eqx + x0 + (2 * fontsize) / 3, eqy + y0);
          check_fl_line(eqx + x0, eqy + y0 - fontsize, eqx + x0 + (2 * fontsize) / 3, eqy + y0 - fontsize);
          check_fl_line(eqx + x0, eqy + y0, eqx + x0 + fontsize / 2, eqy + y0 - fontsize / 2);
          check_fl_line(eqx + x0 + fontsize / 2, eqy + y0 - fontsize / 2, eqx + x0, eqy + y0 - fontsize);
          if (v.size() > 2) { // draw the =
            eqwdata ptmp = Equation_total_size(v[1]);
            if (ptmp.x + ptmp.dx < rightx)
              check_fl_draw("=", eqx + ptmp.x + ptmp.dx - x - 2, eqy + y - ptmp.baseline);
          }
        }
        return;
      }
      if (u == at_abs) {
        y0 = 1 + y - w.y;
        int h = w.dy;
        if (x0 < rightx) {
          check_fl_line(eqx + x0 + 2, eqy + y0 - 1, eqx + x0 + 2, eqy + y0 - h + 3);
          check_fl_line(eqx + x0 + 1, eqy + y0 - 1, eqx + x0 + 1, eqy + y0 - h + 3);
          check_fl_line(eqx + x0 + w.dx - 1, eqy + y0 - 1, eqx + x0 + w.dx - 1, eqy + y0 - h + 3);
          check_fl_line(eqx + x0 + w.dx, eqy + y0 - 1, eqx + x0 + w.dx, eqy + y0 - h + 3);
        }
        return;
      }
      if (u == at_sqrt) {
        y0 = 1 + y - w.y;
        int h = w.dy;
        if (x0 < rightx) {
          check_fl_line(eqx + x0 + 2, eqy + y0 - h / 2, eqx + x0 + fontsize / 2, eqy + y0 - 1);
          check_fl_line(eqx + x0 + fontsize / 2, eqy + y0 - 1, eqx + x0 + fontsize, eqy + y0 - h + 3);
          check_fl_line(eqx + x0 + fontsize, eqy + y0 - h + 3, eqx + x0 + w.dx - 1, eqy + y0 - h + 3);
          ++y0;
          check_fl_line(eqx + x0 + 2, eqy + y0 - h / 2, eqx + x0 + fontsize / 2, eqy + y0 - 1);
          check_fl_line(eqx + x0 + fontsize / 2, eqy + y0 - 1, eqx + x0 + fontsize, eqy + y0 - h + 3);
          check_fl_line(eqx + x0 + fontsize, eqy + y0 - h + 3, eqx + x0 + w.dx - 1, eqy + y0 - h + 3);
        }
        return;
      }
      if (u == at_factorial) {
        check_fl_draw("!", eqx + w.x + w.dx - 4 - x, eqy + y - w.baseline);
        if (!need_parenthesis(tmp.g) && tmp.g != at_pow && tmp.g != at_prod && tmp.g != at_division)
          return;
        if (tmp.x - lpsize < rightx)
          check_fl_draw("(", eqx + tmp.x - x - lpsize, eqy + y - tmp.baseline);
        if (tmp.x + tmp.dx < rightx)
          check_fl_draw(")", eqx + tmp.x + tmp.dx - x, eqy + y - tmp.baseline);
        return;
      }
      if (u == at_integrate) {
        x0 += 2;
        y0 += fontsize / 2;
        if (x0 < rightx) {
          check_fl_arc(eqx + x0, eqy + y0 + fontsize / 3, fontsize / 3, fontsize / 3, 180, 360);
          check_fl_line(eqx + x0 + fontsize / 3, eqy + y0, eqx + x0 + fontsize / 3, eqy + y0 - 2 * fontsize + 4);
          check_fl_arc(eqx + x0 + fontsize / 3, eqy + y0 - 2 * fontsize + 3, fontsize / 3, fontsize / 3, 0, 180);
        }
        if (v.size() != 2) { // if arg has size > 1 draw the d
          eqwdata ptmp = Equation_total_size(v[1]);
          if (ptmp.x < rightx)
            check_fl_draw(" d", eqx + ptmp.x - x - int(check_fl_width(" d")), eqy + y - ptmp.baseline);
        } else {
          eqwdata ptmp = Equation_total_size(v[0]);
          if (ptmp.x + ptmp.dx < rightx)
            check_fl_draw(" dx", eqx + ptmp.x + ptmp.dx - x, eqy + y - ptmp.baseline);
        }
        return;
      }
      if (u == at_division) {
        if (x0 < rightx) {
          int yy = eqy + y0 - 6;
          check_fl_line(eqx + x0 + 2, yy, eqx + x0 + w.dx - 2, yy);
          ++yy;
          check_fl_line(eqx + x0 + 2, yy, eqx + x0 + w.dx - 2, yy);
        }
        return;
      }
      if (u == at_limit && v.size() >= 4) {
        if (x0 < rightx)
          check_fl_draw("lim", eqx + w.x - x, eqy + y - w.baseline);
        gen arg2 = v[1]; // 2nd arg of limit, i.e. the variable
        if (arg2.type == _EQW) {
          eqwdata & varg2 = * arg2._EQWptr;
          if (varg2.x + varg2.dx + 2 < rightx)
            check_fl_draw("->", eqx + varg2.x + varg2.dx + 2 - x, eqy + y - varg2.y);
        }
        if (v.size() >= 5) {
          arg2 = v[2]; // 3rd arg of lim, the point, draw a comma after if dir.
          if (arg2.type == _EQW) {
            eqwdata & varg2 = * arg2._EQWptr;
            if (varg2.x + varg2.dx < rightx)
              check_fl_draw(",", eqx + varg2.x + varg2.dx - x, eqy + y - varg2.baseline);
          }
        }
        return;
      }
      bool parenthesis = true;
      string opstring(",");
      if (u.ptr()->printsommet == & printsommetasoperator || equalposcomp(binary_op_tab(), u))
        opstring = u.ptr()->s;
      else {
        if (u == at_sto)
          opstring = ":=";
        parenthesis = false;
      }
      // int yy=y0; // y0 is the lower coordinate of the whole eqwdata
      // int opsize=int(check_fl_width(opstring.c_str()))+3;
      it = v.begin();
      itend = v.end() - 1;
      // Reminder: here tmp is the 1st arg eqwdata, w the whole eqwdata
      if ((itend - it == 1) && ((u == at_neg) || (u == at_plus) // uncommented for +infinity
        )) {
        if (u == at_neg && need_parenthesis(tmp.g) && tmp.g != at_prod) {
          if (tmp.x - lpsize < rightx)
            check_fl_draw("(", eqx + tmp.x - x - lpsize, eqy + y - tmp.baseline);
          if (tmp.x + tmp.dx < rightx)
            check_fl_draw(")", eqx + tmp.x - x + tmp.dx, eqy + y - tmp.baseline);
        }
        if (w.x < rightx) {
            fl_font(FL_TIMES_BOLD, fontsize);
            check_fl_draw(u.ptr()->s, eqx + w.x - x, eqy + y - w.baseline);
        }
        return;
      }
      // write first open parenthesis
      if (u == at_plus)
        parenthesis = false;
      else {
        if (parenthesis && need_parenthesis(tmp.g)) {
          if (w.x < rightx) {
            int pfontsize = max(fontsize, (fontsize + (tmp.baseline - tmp.y)) / 2);
            fl_font(FL_HELVETICA, pfontsize);
            check_fl_draw("(", eqx + w.x - x, eqy + y - tmp.baseline);
            fl_font(FL_HELVETICA, fontsize);
          }
        }
      }
      for (;;) {
        // write close parenthesis at end
        int xx = tmp.dx + tmp.x - x;
        if (parenthesis && need_parenthesis(tmp.g)) {
          if (xx < rightx) {
            int pfontsize = min(max(fontsize, (fontsize + (tmp.baseline - tmp.y)) / 2), fontsize * 2);
            int deltapary = (2 * (pfontsize - fontsize)) / 3;
            fl_font(FL_HELVETICA, pfontsize);
            check_fl_draw(")", eqx + xx, eqy + y - tmp.baseline + deltapary);
            fl_font(FL_HELVETICA, fontsize);
          }
          xx += rpsize;
        }
        ++it;
        if (it == itend) {
          if (u.ptr()->printsommet == & printsommetasoperator || u == at_sto || equalposcomp(binary_op_tab(), u))
            return;
          else
            break;
        }
        // write operator
        if (u == at_prod) {
          fl_font(FL_TIMES_ITALIC, fontsize);
          check_fl_draw(opstring.c_str(), eqx + xx + 1, eqy + y - tmp.baseline);
        } else {
          gen tmpgen;
          if (u == at_plus && (
              (it->type == _VECT && it->_VECTptr->back().type == _EQW && it->_VECTptr->back()._EQWptr->g == at_neg) ||
              (it->type == _EQW && (is_integer(it->_EQWptr->g) || it->_EQWptr->g.type == _DOUBLE_) && is_strictly_positive(-it->_EQWptr->g, contextptr))
            ))
          ;
          else {
            if (xx + 1 < rightx)
              check_fl_draw(opstring.c_str(), eqx + xx + 1, eqy + y - tmp.baseline);
          }
        }
        // write right parent, update tmp
        tmp = Equation_total_size( * it);
        if (parenthesis && (need_parenthesis(tmp.g))) {
          if (tmp.x - lpsize < rightx) {
            int pfontsize = min(max(fontsize, (fontsize + (tmp.baseline - tmp.y)) / 2), fontsize * 2);
            int deltapary = (2 * (pfontsize - fontsize)) / 3;
            fl_font(FL_HELVETICA, pfontsize);
            check_fl_draw("(", eqx + tmp.x - pfontsize * lpsize / fontsize - x, eqy + y - tmp.baseline + deltapary);
            fl_font(FL_HELVETICA, fontsize);
          }
        }
      } // end for (;;)
      if (w.x < rightx) {
        s = u.ptr()->s;
        fl_font(cst_greek_translate(s), fontsize);
        s += '(';
        check_fl_draw(s.c_str(), eqx + w.x - x, eqy + y - w.baseline);
        fl_font(FL_HELVETICA, fontsize);
      }
      if (w.x + w.dx - rpsize < rightx)
        check_fl_draw(")", eqx + w.x + w.dx - x - rpsize, eqy + y - w.baseline);
      return;
    }
    s = oper.print(contextptr);
    if (w.x < rightx) {
      fl_font(cst_greek_translate(s), fontsize);
      check_fl_draw(s.c_str(), eqx + w.x - x, eqy + y - w.baseline);
      fl_font(FL_HELVETICA, fontsize);
    }
  }

  Equation::Equation(int windowsize, int fontsize, const char * l, const gen & g) {

    const giac::context * contextptr = 0;

    attr = attributs(fontsize, 255, 0);

    if (taille(g, max_prettyprint_equation) < max_prettyprint_equation)
      data = Equation_compute_size(g, attr, windowsize, contextptr);
    else
      data = Equation_compute_size(string2gen("Object_too_large", false), attr, windowsize, contextptr);

    eqwdata total_size = Equation_total_size(data);

    x1 = total_size.x;
    y1 = 0;
    w1 = total_size.dx;
    h1 = total_size.dy;
  }

  Equation::draw() {

    eqwdata total_size = Equation_total_size(data);

    Equation_draw(data, x1, total_size.y + h1, w1, -h1, this);
  }

  Equation::~Equation() {
  }

} // namespace xcas

