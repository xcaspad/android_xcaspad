// Copyright 2008 Google Inc.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package org.giac.calculator.input;

import org.kde.necessitas.mucephi.android_xcas.R;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;

/**
 * Input text view with autocompletion by word.
 * 
 * @author tulaurent@gmail.com (Laurent Tu)
 */
public class InputTextView extends AutoCompleteTextView {

  public InputTextView(Context context, AttributeSet attrs, int defStyle) {
    super(context, attrs, defStyle);
    setSingleLine();
    setupAutoComplete();
	}

	public InputTextView(Context context, AttributeSet attrs) {
    super(context, attrs);
    setSingleLine();
    setupAutoComplete();
	}

	public InputTextView(Context context) {
    super(context);
    setSingleLine();
    setupAutoComplete();
	}

  private static final String[] kKeywords = {
	  "pi","infinity","+infinity", "inf","-infinity", "-inf", "integrate(", "diff(",
	  "abs(","abs(","acos(","acosh(","affixe(","apply(","arg(","asc(","asin(","asinh(","assume(","atan(","atanh(","canonical_form(",
	  "ceil(","char(","coeff(","collect(","concat(","conj(","convert(","coordonees(","cos(","cosh(","cot(","cross(","cumSum(","cyclotomic(",
	  "degree(","desolve(","det(","det(","divis(","divpc(","egcd(","eigenvals(","eigenvects(","exp(","exp2trig(","expand(","expand(","expr(",
	  "factor(","factor(","factorial(","factors(","floor(","frac(","froot(","fsolve(","fsolve(","gcd(","gcd(","genpoly(","getDenom(","getNum(",
	  "head(","hermite(","horner(","hyp2exp(","iabcuv(","idivis(","idn(","iegcd(","ifactor(","ifactors(","im(","image(","int(","int(",
	  "interactive_plotode(","iquo(","iquorem(","irem(","is_prime(","jordan(","ker(","lagrange(","laguerre(","lcm(","lcm(","lcoeff(","limit(",
	  "limit(","limit(","linsolve(","linsolve(","ln(","log(","log10(","makelist(","map(","map(","max(","mid(","min(","newton(","nextprime(","nop(",
	  "nops(","normal(","normal(","odesolve(","op(","partfrac(","pcar(","pcoeff(","peval(","plotfield(","plotode(","pmin(","poly2symb(","poly2symb(",
	  "powmod(","previousprime(","product(","proot(","proot(","propfrac(","ptayl(","quo(","randpoly(","rank(","rank(","ranm(","ratnormal(","re(","rem(",
	  "romberg(","round(","rref(","series(","sign(","simplify(","simult(","sin(","sinh(","size(","size(","solve(","sqrt(","string(","sturmab(","sum(",
	  "symb2poly(","symb2poly(","tail(","tan(","tanh(","taylor(","tchebyshev1(","tchebyshev2(","tcoeff(","tcollect(","texpand(","tlin(","tran(",
	  "trig2exp(","tsimplify(","valuation(","plot(","circle(","line("
  };
  
  private int lastWordStartIndex;
  private int lastCursorPosition;

  /**
   * Setup autocomplete.
   */
  protected void setupAutoComplete() {
    ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(
        this.getContext(), R.layout.list_item, kKeywords);
    setAdapter(arrayAdapter);
    setThreshold(1);
  }

  protected void resetText() {
    setText("", BufferType.EDITABLE);
  }

  @Override
  protected void performFiltering(CharSequence text, int keyCode) {
    // Detect the beginning of the last word in the line.
    Log.i("filtering", "starting");
    lastCursorPosition = getSelectionEnd();
    lastWordStartIndex = 0;
    for (int i = lastCursorPosition-1; i >= 0; i--) {
      char c = text.charAt(i);
      if (c == ' ' ||
    	  c == ',' ||	  
          c == '(' ||
          c == ')' ||
          c == '+' ||
          c == '-' ||
          c == '*' ||
          c == '/' ||
          c == '=' ||
          c == '^') {
        lastWordStartIndex = i+1;
        break;
      }
    }
    
    // Extract last word.
    CharSequence lastWord = null;
    lastWord = text.subSequence(lastWordStartIndex, lastCursorPosition);
    
    // Perform filtering if last word length is bigger than filtering threshold.
    if (lastWord.length() >= this.getThreshold()) {
      super.performFiltering(lastWord, keyCode);
    }
  }

  @Override
  protected void replaceText(CharSequence text) {
    CharSequence currentText = getText();
    
    // Replace currently auto-completed word.
    CharSequence textKeptOnLeft = currentText.subSequence(0, lastWordStartIndex);
    CharSequence textKeptOnRight = currentText.subSequence(lastCursorPosition, currentText.length());
    super.replaceText(textKeptOnLeft + text.toString() + textKeptOnRight);
    
    // Set selection just after replaced word.
    setSelection(lastWordStartIndex + text.length());
  }

}
