import javax.swing.*;

class app {
    public static void main(String args[]) {
       var frame = new JFrame("Swing App");
       frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
       frame.setSize(512, 512);
       frame.setLocationRelativeTo(null);
       var button = new JButton("Click me!");
       button.addActionListener(e -> { System.out.println("Clicked"); });
       frame.getContentPane().add(button);
       frame.setVisible(true);
    }
}