function toggleComments(){
	var commentsElements = document.getElementsByClassName("comments");
	for(i=0;i<commentsElements.length;i++){
		if(commentsElements[i].hidden === false){
			commentsElements[i].hidden=true;
			document.getElementById("commentsBtn").value="Show Comments";
		}
		else{
			commentsElements[i].hidden = false;
			document.getElementById("commentsBtn").value="Hide Comments";
			
		}
	}
}


function toggleFunctions(){
	var functionElements = document.getElementsByClassName("function");
	for(i=0;i<functionElements.length;i++){
		if(functionElements[i].hidden === false){
			functionElements[i].hidden=true;
			document.getElementById("functionBtn").value="Show Functions";
		}
		else{
			functionElements[i].hidden = false;
			document.getElementById("functionBtn").value="Hide Functions";
			
		}
	}
}


function toggleClasses(){
	var classElements = document.getElementsByClassName("cclass");
	for(i=0;i<classElements.length;i++){
		if(classElements[i].hidden === false){
			classElements[i].hidden=true;
			document.getElementById("classBtn").value="Show Classes";
		}
		else{
			classElements[i].hidden = false;
			document.getElementById("classBtn").value="Hide Classes";
			
		}
	}
}