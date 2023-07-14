#![allow(non_snake_case)]
use dioxus::prelude::*;

fn main() {
    dioxus_desktop::launch(App);
}

fn App(cx: Scope) -> Element {
    cx.render(rsx! {
        button {
            onclick: |_| {
                println!("Clicked");
            },
            "Click me!"
        }
    })
}