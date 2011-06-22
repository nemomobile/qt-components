/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
/* This function generates menus and search box in narrow/slim fit mode */
var narrowInit = function() {
  /* 1: Create search form */ 
  var narrowSearch = $('<div id="narrowsearch"></div>');
  var searchform = $("#qtdocsearch");
  narrowSearch.append(searchform);
  $("#qtdocheader .content .qtref").after(narrowSearch);

  /* 2: Create dropdowns */ 
  var narrowmenu = $('<ul id="narrowmenu" class="sf-menu"></ul>');

  /* Lookup */ 
  var lookuptext = $("#lookup h2").attr("title");
  $("#lookup ul").removeAttr("id");
  $("#lookup ul li").removeAttr("class");
  $("#lookup ul li").removeAttr("style");
  var lookupul = $("#lookup ul");
  var lookuplist = $('<li></li>');
  var lookuplink = $('<a href="#"></a>');
  lookuplink.append(lookuptext);
  lookuplist.append(lookuplink);
  lookuplist.append(lookupul);
  narrowmenu.append(lookuplist);

  /* Topics */ 
  var topicstext = $("#topics h2").attr("title");
  $("#topics ul").removeAttr("id");
  $("#topics ul li").removeAttr("class");
  $("#topics ul li").removeAttr("style");
  var topicsul = $("#topics ul");
  var topicslist = $('<li></li>');
  var topicslink = $('<a href="#"></a>');
  topicslink.append(topicstext);
  topicslist.append(topicslink);
  topicslist.append(topicsul);
  narrowmenu.append(topicslist);

  /* Examples */ 
  var examplestext = $("#examples h2").attr("title");
  $("#examples ul").removeAttr("id");
  $("#examples ul li").removeAttr("class");
  $("#examples ul li").removeAttr("style");
  var examplesul = $("#examples ul");
  var exampleslist = $('<li></li>');
  var exampleslink = $('<a href="#"></a>');
  exampleslink.append(examplestext);
  exampleslist.append(exampleslink);
  exampleslist.append(examplesul);
  narrowmenu.append(exampleslist);

  $("#shortCut").after(narrowmenu);
  $('ul#narrowmenu').superfish({
    delay: 100,
    autoArrows: false,
    disableHI: true
  });
}

/* Executes on doc ready */
$(document).ready(function(){
	/* check if body has the narrow class */
	if ($('body').hasClass('narrow')) {
		/* run narrowInit */
		narrowInit();
	}
 
	/* messure window width and add class if it is smaller than 600 px */
	if($(window).width()<600) {
		$('body').addClass('narrow');
		/* if the search box contains */
		if ($("#narrowsearch").length == 0) {
			/* run narrowInit */
			narrowInit();
		}
	  }
	  else { /* if the window is wider than 600 px, narrow is removed */
		$('body').removeClass('narrow');
		if ($("#narrowsearch").length == 0) {
		}
	}
});
/* binding resize event to this funciton */
$(window).bind('resize', function () {
	/*  if the window is wider than 600 px, narrow class is added */
	if($(window).width()<600) {
		$('body').addClass('narrow');
		if ($("#narrowsearch").length == 0) {
		  narrowInit();
		}
	}
	else {
		/* else we remove the narrow class */
		$('body').removeClass('narrow');
  }
});

	$('#narrowsearch').keyup(function () {
		/* extract the search box content */
	  var searchString = $('#narrowsearch').val();
	  /* if the string is less than three characters */
	  if ((searchString == null) || (searchString.length < 3)) {
			/* remove classes and elements*/
			$('#narrowsearch').removeClass('loading');
			$('.searching').remove(); 
			/*  run CheckEmptyAndLoadList */
			CheckEmptyAndLoadList();
			
			$('.report').remove();
			return;
	   }
	   /* if timer checks out */
		if (this.timer) clearTimeout(this.timer);
		this.timer = setTimeout(function () {
			/* add loading image by adding loading class */
			$('#narrowsearch').addClass('loading');
			$('.searching').remove(); 

			/* run the actual search */
		   $.ajax({
			contentType: "application/x-www-form-urlencoded",
			url: 'http://' + location.host + '/nokiasearch/GetDataServlet',
			data: 'searchString='+searchString,
			dataType:'xml',
			type: 'post',	 
			success: function (response, textStatus) {
				/* on success remove loading img */
				$('.searching').remove(); 
				$('#narrowsearch').removeClass('loading');
				processNokiaData(response);
			}     
		  });
		}, 500); /* timer set to 500 ms */
	});
