$(document).ready(function () {
    // Select all buttons with type="options"
    // $('button[type="options"], button[type="accept"]').each(function () {
    //     var label = $(this).attr('label'); // Extract the label attribute
    //     // Assuming the <a> tag is supposed to be somewhere related but not inside the button
    //     // For demonstration, we're selecting an <a> tag following this button
    //     $(this).find('a').first().html(label); // Set the label as the innerHTML of the <a> tag
    //     $(this).children('prev').first().html(label);
    // });
    // $('#menu').toggleClass('d-none');
    // $('#char').toggleClass('d-none');
    $('a[data-content="template"]').each(function () {
        let to = $(this).attr('label');
        let content = $('#menu').html().replaceAll('$(to)', to);
        $(this).attr('data-html', 'true');
        $(this).attr('data-content', content);
    });
    $('a[data-content="char"]').each(function () {
        let to = $(this).attr('label');
        let content = $('#char').html().replaceAll('$(to)', to);
        $(this).attr('data-html', 'true');
        $(this).attr('data-content', content);
    });
    $('.popover-dismiss').popover({
        trigger: 'focus'
    });
    $('[data-toggle="popover"]').popover();
    $('#login').click(function () {
        nn = $('input[name="nn"]').val();
        pass = $('input[name="pass"]').val();
        href = $(this).attr('href').replaceAll('$(nn)', nn).replaceAll('$(pass)', pass);
        window.location.href = href;
    });
    $('#reg').click(function () {
        nn = $('input[name="nn"]').val();
        href = $(this).attr('href').replaceAll('$(nn)', nn);
        window.location.href = href;
    });
    $('button[type="admin"]').click(function () {
        inp = $('input[name="inp"]').val();
        val = $('input[name="val"]').val();
        newval = $('input[name="new"]').val();
        href = $(this).attr('href').replaceAll('$(inp)', inp).replaceAll('$(val)', val).replaceAll('$(new)', newval);
        window.location.href = href;
    });
});