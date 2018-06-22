/*
 *  Copyright (C) 2011 Leonel Hernández Sandoval.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

package org.kde.necessitas.mucephi.android_xcas;

/**
 * Created by leonel on 29/01/18.
 */

public class TestsOperations {

    public static String[] operations = {
            "phi, psi, beta",
            "integrate(1/sin(x),x)",
            "integrate(x**2/sin(x),x)",
            "[X**2, integrate(1/sin(x),x),[[2.1,3.4,5.4],[5.7,6.2,7.5],[945.4,6,7]], integrate(x**y/sin(x),x)]",
            //"subst(product(x))",
            "cFactor(x^4+1,sqrt(2))",
            "cFactor(x^4-1)",
            "cFactor(x^4+1)",
            "zeros(ln(x)^2-2)",
            "cZeros(x*(exp(x))^2-2*x-2*(exp(x))^2+4)",
            "subst('integrate(sin(x^2)*x,x,0,pi/2)',x=sqrt(t))",
            "fdistrib((x+1)*(x-2))", //aqui hay un error en el minus
    /*10*/
            "canonical_form(x^2-6*x+1)",
            "mult_conjugate((2+sqrt(2))/(2+sqrt(3)))",
            "mult_conjugate((2+sqrt(2))/(sqrt(2)+sqrt(3)))",
            "mult_conjugate((2+sqrt(2))/2)",
            "tablefunc(sin(n),n,0,1)",
            "tableseq(sin(n),n,3.5)",
            "feuille(pgcd)[1]", //aqui hay error
            "f(x):=x^2+x*cos(x), function_diff(f)",
            "diff(x*y^2*z^3+x*y,[x,y,z])",
            "normal(diff((1)/(x^2+2),x,x,x))", //otra vez error esta, en el minus
            "integrate(1/(sin(x)+2),x,0,2*pi)",
    /*20*/
            "partfrac(1/(n^3-n))",
            "sum_riemann(32*n^3/(16*n^4-k^4),[n,k])",
            "limit((n*tan(x)-tan(n*x))/(sin(n*x)-n*sin(x)),x=0)",
            "texpand((sin(3*x)+sin(7*x))/sin(5*x))", //falta el menos
            "q:=1/8*fft([11,10,17,24,32,26,23,19])", //muy larga no lo pinto todo
            "lin(sinh(x)^2)",
            "r2e(%%%{1,[2,0]%%%}+%%%{-1,[1,1]%%%}+%%%{2,[0,1]%%%},[x,y])",
            "collect(x^3-2*x^2+1,sqrt(5))", //minus
            "divis(x^4-1)", //se sale de la pantalla
            "complexroot(x^3+1,0.1,-1,1+2*i)",
    /*30*/
            "idn(7)",
            "ranm(2,4,'randnorm(0,1)')", //error los numeros se empalman
            "hessian(2*x^2*y-x*z^3, [x,y,z])",
            "solve([x^2+y=2,x+y^2=2],[x,y])",
            "solve([x^2+y+z=42,x+y^2-z=5, x+y+z^2=8],[x,y,z])",
            "cSolve(x^4-1=3)",
            "desolve(x*y'-2*y-x*exp(4/x)*y^3,y)",
            "sum(1/n^2,n,1,17)",
            "matrix(2,3,(j,k)->1/(j+k+1))"
    };
}
