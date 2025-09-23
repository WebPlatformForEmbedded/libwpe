$(document).ready(function() {
	setupFilters();

    /* TODO: get rid of this somehow */
	$("#toc-wrapper").mCustomScrollbar({"scrollInertia": 0,
					    "theme": "dark",
					    "mouseWheel":{ "preventDefault": true },
					    "documentTouchScroll": false});

	$("#main").swipe({
		swipe:function(event, direction, distance, duration, fingers)
		{
			console.log(direction, distance, duration, fingers);
			if (direction == "right") {
				if ($("body").hasClass("toc-expanded"))
					$("body").removeClass("toc-expanded");
				else
					$("body").addClass("sitenav-expanded");
				return false;
			}
			if (direction == "left") {
				if ($("body").hasClass("sitenav-expanded"))
					$("body").removeClass("sitenav-expanded");
				else
					$("body").addClass("toc-expanded");
				return false;
			}
		},
		allowPageScroll: "vertical",
		fallbackToMouseEvents: false,
		excludedElements: "button, input, select, textarea, a, .noSwipe, pre",
	});

	$("#body").click(function(e) {
		$("body").removeClass("toc-expanded");
		$("body").removeClass("sitenav-expanded");
	});

	$("#sidenav-toggle").click(function(e) {
		$("body").removeClass("toc-expanded");
		$("body").toggleClass("sitenav-expanded");
	});

	$("#toc-toggle").click(function(e) {
		$("body").removeClass("sitenav-expanded");
		$("body").toggleClass("toc-expanded");
	});

    /* Communicate context to our navigation iframe.
     *
     * As it may get loaded before or after us, we communicate
     * it too upon reception of a sitenav-status ready message.
     */
    const frame = document.getElementById('sitenav-frame');
    let msg = {"hotdoc/sitenav-action": "unfold"}
    Object.assign(msg, utils.hd_context);
    frame.contentWindow.postMessage(msg, '*');

    $(window).on("message", function(e) {
        console.log("main window got message", e.originalEvent.data);

        let msg = e.originalEvent.data;

        if (msg["hotdoc/sitenav-status"] == "ready") {
            let msg = {"hotdoc/sitenav-action": "unfold"}
            Object.assign(msg, utils.hd_context);
            frame.contentWindow.postMessage(msg, '*');
        }
    });
});
