Gömülü Sistemler İçin Dinamik Tensör ve Nicemleme (Quantization) Projesi
Proje Vizyonu
Bu çalışma, kaynak kısıtlı mikrodenetleyici ortamlarında (TinyML) bellek verimliliğini maksimize etmek amacıyla geliştirilmiş bir yazılım mimarisidir. Standart yüksek hassasiyetli veri tiplerinin yarattığı bellek dar boğazını aşmak için dinamik veri yapısı yönetimi ve veri sıkıştırma tekniklerini bir araya getirir.

Temel Bileşenler ve Mühendislik Yaklaşımı
1. Tensör Yapısı ve Bellek Yönetimi
Tensörler, yapay zeka modellerinin veri işleme birimleridir. Bu projede kullanılan tensör yapısı, C dilinin sunduğu dinamik bellek yönetimi imkanlarıyla tasarlanmıştır. Bu sayede matris boyutları uygulama esnasında (runtime) belirlenerek sistemin esnekliği artırılmıştır.

2. Union ve Çoklu Veri Tipi Desteği (Puanlama Kriteri)
Projenin en kritik optimizasyon noktası olan Union yapısı, farklı hassasiyetteki veri işaretçilerinin bellekte tam olarak aynı başlangıç adresini paylaşmasını sağlar.

Bu yöntemle, 32-bit Float veya 8-bit Integer verileri için ayrı ayrı bellek alanları ayırmak yerine, tek bir alan üzerinden tip dönüşümü yapılır.

Sonuç olarak RAM üzerinde büyük bir yer tasarrufu sağlanmış olur.

3. Nicemleme (Quantization) Teknolojisi (Puanlama Kriteri)
Quantization, yüksek çözünürlüklü (32-bit) verilerin, doğruluğu optimize edilmiş bir ölçeklendirme katsayısı (scale) kullanılarak daha düşük çözünürlüklü (8-bit) verilere indirgenmesi işlemidir.

Avantajı: Bellek kullanımını teorik olarak 4 kat azaltır.

Uygulama: Veriler bir ölçek katsayısına bölünerek tamsayıya yuvarlanır ve bu sayede işlemci yükü ile RAM kullanımı minimize edilir.

Analiz ve Doğrulama (CLion Debugger)
Projenin doğruluğu, CLion geliştirme ortamının gelişmiş hata ayıklama araçlarıyla test edilmiştir:

Matris Yönetimi: Debugger üzerindeki değişken izleme (variables) sekmesi kullanılarak matris indislerinin ve boyutlarının doğru yönetildiği teyit edilmiştir.

Tip Dönüşümü: Bellek görünümü (Memory View) üzerinden, bir verinin Float formatından Integer formatına dönüştüğü anlık olarak gözlemlenmiş ve Union yapısının bellek adreslerini nasıl paylaştırdığı doğrulanmıştır.

Sonuç
Bu uygulama; ham sensör verilerinin dinamik olarak işlenmesi, sıkıştırılması ve kısıtlı donanımlarda yüksek performansla saklanabilmesi için uçtan uca bir çözüm sunmaktadır. Gömülü yazılım prensipleri ile yapay zeka veri yapılarının entegrasyonuna bir örnek teşkil eder.
