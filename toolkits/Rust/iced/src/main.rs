use iced::{Element, Sandbox, Settings, window};

pub fn main() -> iced::Result {
    let settings = Settings {
        window: window::Settings {
            size: (512, 512),
            ..Default::default()
        },
        ..Default::default()
    };
    Hello::run(settings)
}

struct Hello;

impl Sandbox for Hello {
    type Message = ();

    fn new() -> Hello {
        Hello
    }

    fn title(&self) -> String {
        String::from("A cool application")
    }

    fn update(&mut self, _message: Self::Message) {
        // This application has no interactions
    }

    fn view(&self) -> Element<Self::Message> {
        "Hello, world!".into()
    }
}
