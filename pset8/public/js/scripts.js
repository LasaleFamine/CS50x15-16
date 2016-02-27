/* global google */
/* global _ */
/**
 * scripts.js
 *
 * Computer Science 50x16
 * Problem Set 8
 * Alessio Occhipinti3
 * 
 *
 * Global JavaScript.
 */

// Google Map
var map;

// markers for map
var markers = [];

// info window

var info = new google.maps.InfoWindow();


// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 42.3620, lng: -71.0830}, // Cambridge, Massachusetts
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);
    
    /*** MOD init updateLastSearch ***/
    Cookies.remove('last_q0');
    var lastQ = Cookies.getJSON('last_q');
    updateLastSearch(lastQ);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    var myLatlng;

    myLatlng = new google.maps.LatLng(place.latitude, place.longitude);
    
    // Prepare the maker
    var marker = new MarkerWithLabel({
        position: myLatlng,
        draggable: false,
        raiseOnDrag: true,
        map: map,
        labelContent: place.place_name + ', ' + place.admin_name1,
        labelAnchor: new google.maps.Point(22, 0),
        labelClass: "labels", // the CSS class for the label
        labelStyle: {opacity: 0.75},
        icon: "../img/news.png"
    });
    
    // Add marker to global var
    markers.push(marker);

    google.maps.event.addListener(marker, "click", function (e) { getContent(place.postal_code, marker) });

    // To add the marker to the map, call setMap();
    marker.setMap(map);
    
}



/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {
        update();
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // remove markers whilst dragging
    google.maps.event.addListener(map, "dragstart", function() {
        removeMarkers();
    });

    // configure typeahead
    // https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md
    $("#q").typeahead({
        autoselect: true,
        highlight: true,
        minLength: 1
    },
    {
        source: search,
        templates: {
            empty: "no places found yet",
            suggestion: _.template('<p><%- place_name %>, <%- admin_name1 %> <span class="small text-muted"><%- postal_code %></span></p>')
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // ensure coordinates are numbers
        var latitude = (_.isNumber(suggestion.latitude)) ? suggestion.latitude : parseFloat(suggestion.latitude);
        var longitude = (_.isNumber(suggestion.longitude)) ? suggestion.longitude : parseFloat(suggestion.longitude);
        
        /***** MOD adding cookies for last search *****/
        var last_q = Cookies.getJSON('last_q');
        if (last_q) {
            Cookies.set('last_q0', last_q);
            updateLastSearch(last_q);
        }
        
        Cookies.set('last_q', suggestion);

        // set map's center
        map.setCenter({lat: latitude, lng: longitude});

        // update UI
        update();
    });
    
     /**** MOD bind click on last search *****/
    $("#lastSearch").on("click", '.last-q', function(e) {
        e.preventDefault();
        
        var latitude = $(this).attr('data-lat');
        var longitude = $(this).attr('data-lng');
        
        // ensure coordinates are numbers
        var latitude = (_.isNumber(latitude)) ? latitude : parseFloat(latitude);
        var longitude = (_.isNumber(longitude)) ? longitude : parseFloat(longitude);

        // set map's center
        map.setCenter({lat: latitude, lng: longitude});

        // update UI
        update();
    });
    

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        hideInfo();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Hides info window.
 */
function hideInfo()
{
    info.close();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    for (var i = 0; i < markers.length; i++) {
        markers[i].setMap(null);
    }
    markers = [];
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, cb)
{
    // get places matching query (asynchronously)
    var parameters = {
        geo: query
    };
    $.getJSON("search.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // call typeahead's callback with search results (i.e., places)
        cb(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) === "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='img/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update() 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();
    

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON("update.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // remove old markers from map
        removeMarkers();

        // add new markers to map
        for (var i = 0; i < data.length; i++)
        {
            addMarker(data[i]);
        }
     })
     .fail(function(jqXHR, textStatus, errorThrown) {

         // log error to browser's console
         console.log(errorThrown.toString());
     });
}



/*
 * Get and show content function (callback for listener on click marker)
 * @Alessio Occhipinti
 */ 
function getContent(postal_code, marker, content)
{

    // Start showInfo with loader icon    
    var content;
    showInfo(marker, content);
    
     // Open the ul tag for the content
    content = '<ul>';
    
    // get news with postal code
    var parameters = {
        geo: postal_code
    };
    $.getJSON("articles.php", parameters)
        .done(function(data, textStatus, jqXHR) {
            // If no news, show Nothing found
            if (data.length == 0) {
                showInfo(marker, "Nothing found");
                return;
            }
            
            // Else show the news
            for (var i = 0; i < data.length; i++)
            {   
                // Add elemnts to ul
                content += '<li><a href="' + data[i].link + '" trget="_blank">' + data[i].title + '</a></li>';
            }
            // Closing ul
            content += '</ul>';
            showInfo(marker, content);
         })
         .fail(function(jqXHR, textStatus, errorThrown) {
            // log error to browser's console
            console.log(errorThrown.toString());
            // On fail show error
            showInfo(marker, "Error getting news. Retry");
            return;
         });
    
}

/*
 * Update last search box
 * Alessio Occhipinti
 */
 
 function updateLastSearch(lastQ) {
    
    if(lastQ) {
         var template = '<a class="last-q" href="#" data-lat="' + lastQ.latitude + '" data-lng="' + lastQ.longitude + '">' + lastQ.place_name + ', ' + lastQ.admin_name1 + ', ' + lastQ.postal_code; + '</a>';
         $('#lastSearch').html(template);
    }
 }
