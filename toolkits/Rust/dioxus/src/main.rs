use dioxus::prelude::*;
use dioxus_desktop as dd;

fn main() {
    dd::launch_cfg(
        app,
        dd::Config::default().with_window(
            dd::WindowBuilder::new()
                .with_inner_size(dd::PhysicalSize::new(512, 512)),
        ),
    );
}

fn app(cx: Scope) -> Element {
    cx.render(rsx! {
        button {
            onclick: |_| {
                println!("Clicked");
            },
            "Click me!"
        }
    })
}
