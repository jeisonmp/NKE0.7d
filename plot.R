
library(ggplot2)
data2<-read.table("plot.txt")
png(filename="plot.png",width=1169,height=827)
ggplot(data2, aes(colour=tarefa)) + 
  geom_segment(aes(x=start, xend=end, y=tarefa, yend=tarefa), size=5) +
  theme_bw() + labs(title="Scheduler", x = "Decorrido", y = "Tarefa") +
  theme(panel.grid.minor =   element_blank(),
        panel.grid.major =   element_line(colour = "grey",size=0.4)) +
  scale_x_continuous(breaks = c(0,data2$end)) 
dev.off()

#png(filename="/home/uow/Documentos/PaNKE0.4e/plot2.png",width=13,height=10)
#ggplot(data2, aes(colour=tarefa)) + 
#  geom_segment(aes(x=start, xend=end, y=tarefa, yend=tarefa), size=5) +
#  theme_bw() + labs(title="Scheduler", x = "Decorrido", y = "Tarefa") +
#  theme(panel.grid.minor =   element_blank(),
#        panel.grid.major =   element_line(colour = "grey",size=0.4)) +
#  scale_x_continuous(breaks = seq(min(data$start),max(data2$end),5))
#dev.off()

