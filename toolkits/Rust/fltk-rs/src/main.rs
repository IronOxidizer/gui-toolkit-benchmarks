use fltk::{prelude::*, app::App, button::Button, window::Window};

fn main() {
    let a = App::default();
    let mut win = Window::new(0, 0, 512, 512, "Fltk Program");
    let mut btn = Button::default_fill().with_label("Click me!");
    btn.set_callback(|_| eprintln!("Clicked!"));
    win.end();
    win.show();
    a.run().unwrap();
}
