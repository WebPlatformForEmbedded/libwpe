function setActiveStyleSheet(title) {
  localStorage.setItem("hotdoc.style", title);

  var i, a;
  for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
    if (!a.hasAttribute('rel')) {
      continue;
    }

    if(a.getAttribute("rel").indexOf("style") != -1 && a.getAttribute("title")) {
      a.disabled = true;
      if(a.getAttribute("title") == title) a.disabled = false;
    }
  }

  const frame = document.getElementById('sitenav-frame');

  if (frame) {
    let msg = {"hotdoc/sitenav-action": "update-style"}
    frame.contentWindow.postMessage(msg, '*');
  }
}

function getActiveStyleSheet() {
  var i, a;
  for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
    if (!a.hasAttribute('rel')) {
      continue;
    }

    if(a.getAttribute("rel").indexOf("style") != -1 && a.getAttribute("title") && !a.disabled) return a.getAttribute("title");
  }
  return null;
}

function getPreferredStyleSheet() {
  var i, a;
  for(i=0; (a = document.getElementsByTagName("link")[i]); i++) {
    if (!a.hasAttribute('rel')) {
      continue;
    }

    if(a.getAttribute("rel").indexOf("style") != -1
       && a.getAttribute("rel").indexOf("alt") == -1
       && a.getAttribute("title")
       ) return a.getAttribute("title");
  }
  return null;
}

function setPreferredStyleSheet() {
  const stored = localStorage.getItem("hotdoc.style");
  const title = stored ? stored : getPreferredStyleSheet();
  setActiveStyleSheet(title);
}

window.onload = function(e) {
  setPreferredStyleSheet();
}

// window.onunload = function(e) {
//   var title = getActiveStyleSheet();
//   localStorage.setItem("hodoc.style", title);
// }

setPreferredStyleSheet();

$(document).ready(function() {
  $('#lightmode-icon').click(function() {
    if (getActiveStyleSheet() == 'dark') {
      setActiveStyleSheet('light');
    } else {
      setActiveStyleSheet('dark');
    }
  });
});
