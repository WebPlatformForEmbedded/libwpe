function update_url() {
  var parsedUri = utils.parseUri(window.location.href);
  var updatedUri = parsedUri['scheme'] + '://' + parsedUri['authority'] + parsedUri['path'];

	if (utils.hd_context.gi_languages.length) {
    updatedUri += "?gi-language=" + utils.hd_context.gi_language;
  }

  if (parsedUri['fragment'] != undefined)
    updatedUri += '#' + parsedUri['fragment'];
  history.replaceState({}, document.title, updatedUri);
}

function scroll_if_anchor(href, initial) {
	var fromTop = parseInt($('body').css('padding-top'));
	href = typeof(href) == "string" ? href : $(this).attr("href");

	if (href == undefined)
		return;

	var dest = utils.parseUri(href);

	if (utils.uri_is_in_this_page(href)) {
		href = "#" + dest.fragment;
	}

	if(href.indexOf("#") == 0) {
		var $target = $(href.replace( /(:|\.|\[|\]|,)/g, "\\$1"));

    if (!$target.length)
      $target = $('a[name="' + href.substring(1) + '"]');

		if($target.length) {
			$('html, body').scrollTop($target.offset().top - 3 - fromTop);
			if(!initial && history && "pushState" in history) {
				history.pushState({}, document.title, window.location.pathname + href);
        update_url();
        return false;
			}
    }
  }

  update_url();
}

window.addEventListener('popstate', update_url);
$(document).ready(function() {
  scroll_if_anchor(window.location.hash, true);
  $("body").on("click", "a[href]", scroll_if_anchor);
  window.onhashchange = function() {
    scroll_if_anchor(location.hash, false);
  }
});
