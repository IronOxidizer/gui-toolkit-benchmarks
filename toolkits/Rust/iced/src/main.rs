use iced::{Text, Sandbox, Element, Settings};
fn main() -> iced::Result
{
    Hello::run(Settings::default())
}

struct Hello;

impl Sandbox for Hello
{
	type Message = ();

	fn new() -> Self
	{
		Self
	}

	fn title(&self) -> String
	{
		String::from("Title")
	}
	fn update(&mut self, _message: Self::Message)
	{

	}
	fn view(&mut self) -> Element<Self::Message>
	{
		Text::new("Hello world").into()
	}
}
    
