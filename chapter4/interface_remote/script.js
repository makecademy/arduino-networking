// When interface is loaded
window.onload = function() {
    $.get( "command.php", {command: "/mode/7/o"} );
}

// Function to control the relay
function buttonClick(clicked_id){

    if (clicked_id == "1"){
        $.get( "command.php", {command: "/digital/7/1"} );  
    } 

    if (clicked_id == "2"){
        $.get( "command.php", {command: "/digital/7/0"} );  
    } 
}