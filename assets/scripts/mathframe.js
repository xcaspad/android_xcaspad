
function writeMathComputation(expressionID){
	
	var showresult = document.createElement("div");
    showresult.setAttribute("class", "rescontainer");
    mathArea.appendChild(showresult);


    var elExp = document.createElement("div");
    elExp.setAttribute("class", "printout expression");
    elExp.innerHTML = XCAS.getParsedExpression(expressionID);
    elExp.onclick = function(){
        XCAS.retriveExpression("source", expressionID);
    };


	MathJax.Hub.Queue(["Typeset",MathJax.Hub, elExp], function(){
		var elComp = document.createElement("div");
		elComp.innerHTML = XCAS.getResultExpression(expressionID);
		
		elComp.setAttribute("class", "printout success");
		elComp.onclick = function(){
				XCAS.retriveExpression("result", expressionID);
		};

		MathJax.Hub.Queue(["Typeset",MathJax.Hub, elComp], function(){
			showresult.innerHTML = "";
			showresult.appendChild(elExp);
			showresult.appendChild(elComp);
			//XCAS.dismissProgress();
			window.scrollTo(0, document.body.scrollHeight);
		});
	});
	
	/*MathJax.Hub.Register.StartupHook("End",function () {
	
		var exp = XCAS.getParsedExpression(expressionID);
	
		var TEX = new LaTeX2HTML5.TeX({tagName:'div',className:'printout expression',latex:exp });
		TEX.$el.click(function(){XCAS.retriveExpression("source", expressionID);});
		TEX.render();
		
		$(mathArea).append(TEX.$el);
		
		var res = XCAS.getResultExpression(expressionID);
	
		TEX = new LaTeX2HTML5.TeX({tagName:'div',className:'printout success',latex:res});
		TEX.$el.click(function(){XCAS.retriveExpression("result", expressionID);});
		TEX.render();
		
		$(mathArea).append(TEX.$el);
		
	});*/
}


