function md_convert(id, md_url)
{
    console.log("Script begining")

    var element = document.getElementById(id);

    var converter = new showdown.Converter({tables: true, strikethrough: true});

    var markdown_text = "";

    $.ajax( { url: md_url,
              type: 'get',
              dataType: 'text',
              async: false,
              success: function(data) { markdown_text = data; }
    });

    element.innerHTML = converter.makeHtml(markdown_text)
}
