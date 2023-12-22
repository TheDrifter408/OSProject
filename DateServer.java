import java.net.*;
import java.io.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class DateServer {
    
    private static class ClientService implements Runnable {
        private final Socket client;

        public ClientService(Socket client) {
            this.client = client;
        }

        @Override
        public void run() {
            try (
                BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
                PrintWriter out = new PrintWriter(client.getOutputStream(), true)
            ) {
                // read client input
                // String client input = in.readLine();

                // write the Date to the socket
                out.println(new java.util.Date().toString());
            } catch (IOException ioe) {
                System.err.println(ioe);
            } finally {
                try {
                    client.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    public static void main(String[] args) {
        ExecutorService executorService = Executors.newFixedThreadPool(10);

        try (ServerSocket sock = new ServerSocket(6013)) {
            while (true) {
                Socket client = sock.accept();
                executorService.submit(new ClientService(client));
            }
        } catch (IOException ioe) {
            System.err.println(ioe);
        } finally {
            executorService.shutdown();
        }
    } 
}
