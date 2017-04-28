/****************************************************************************************



****************************************************************************************/




#include <tclap/CmdLine.h>
#include <tclap/ArgException.h>
#include <fstream>

#include "point_cloud/point_cloud_builder.h"

#include "compression/encode.h"
#include "core/cycle_timer.h"
//#include "compression/attributes/attributes_encoder.h"
//#include "compression/config/compression_shared.h"
//#include "compression/config/encoder_options.h"
//#include "core/encoder_buffer.h"


#include "ExporterConfig.h"
#include "awdbuf.h"



struct HEADER {
    char numAttribs;
    short bufferType;
};


struct Options {
    Options();
    
    int pos_quantization_bits       ;
    int tex_coords_quantization_bits;
    int normals_quantization_bits   ;
    int compression_level           ;
    std::string input;
    std::string output;
};


Options::Options()
: pos_quantization_bits(14),
tex_coords_quantization_bits(12),
normals_quantization_bits(10),
compression_level(7) {}



void PrintOptions(const draco::PointCloud &pc, const Options &options) {
    printf("Encoder options:\n");
    printf("  Compression level = %d\n", options.compression_level);
    if (options.pos_quantization_bits <= 0) {
        printf("  Positions: No quantization\n");
    } else {
        printf("  Positions: Quantization = %d bits\n",
               options.pos_quantization_bits);
    }
    
    if (pc.GetNamedAttributeId(draco::GeometryAttribute::TEX_COORD) >= 0) {
        if (options.tex_coords_quantization_bits <= 0) {
            printf("  Texture coordinates: No quantization\n");
        } else {
            printf("  Texture coordinates: Quantization = %d bits\n",
                   options.tex_coords_quantization_bits);
        }
    }
    
    if (pc.GetNamedAttributeId(draco::GeometryAttribute::NORMAL) >= 0) {
        if (options.normals_quantization_bits <= 0) {
            printf("  Normals: No quantization\n");
        } else {
            printf("  Normals: Quantization = %d bits\n",
                   options.normals_quantization_bits);
        }
    }
    printf("\n");
}



int EncodePointCloudToFile(const draco::PointCloud &pc,
                           const draco::EncoderOptions &options,
                           const std::string &file) {
    draco::CycleTimer timer;
    // Encode the geometry.
    draco::EncoderBuffer buffer;
    timer.Start();
    if (!draco::EncodePointCloudToBuffer(pc, options, &buffer)) {
        printf("Failed to encode the point cloud.\n");
        return -1;
    }
    timer.Stop();
    // Save the encoded geometry into a file.
    std::ofstream out_file(file, std::ios::binary);
    if (!out_file) {
        printf("Failed to create the output file.\n");
        return -1;
    }
    out_file.write(buffer.data(), buffer.size());
    printf("Encoded point cloud saved to %s (%" PRId64 " ms to encode)\n",
           file.c_str(), timer.GetInMs());
    printf("\nEncoded size = %zu bytes\n\n", buffer.size());
    return 0;
}





int main(int argc, char** argv)
{
    
    Options options;
    
    
    
    
    // Parse cmdlne args using tclap
    try {
        

        TCLAP::CmdLine cmd("draco buffer encoder", ' ', DracoBufferEncoder_VERSION_STR);

        
        TCLAP::ValueArg<std::string> inputArg( "i", "input", "file's path to convert (stdin if not set)", false, "", "input"  );
        
        TCLAP::ValueArg<std::string> outputArg("o", "output","output file's path (stdout if not set)"   , false, "", "output" );
        
        
        TCLAP::ValueArg<uint> pos_quantization_bits       ("p","qp","quantization bits for the position, default 14",           false, 14, "pos");
        TCLAP::ValueArg<uint> tex_coords_quantization_bits("t","qt","quantization bits for the texture coordinate, default 12", false, 12, "tc");
        TCLAP::ValueArg<uint> normals_quantization_bits   ("n","qn","quantization bits for the normal vector, default 10",      false, 10, "tc");
        TCLAP::ValueArg<uint> compression_level           ("q","cl","compression level [0-10], most=10, least=0, default 7",    false, 7, "tc");
        
        cmd.add( inputArg );
        cmd.add( outputArg );
        cmd.add( pos_quantization_bits        );
        cmd.add( tex_coords_quantization_bits );
        cmd.add( normals_quantization_bits    );
        cmd.add( compression_level            );
        
        
        cmd.parse( argc, argv );
        
        // Get the value parsed by each arg.
        options.input           = inputArg.getValue();
        options.output          = outputArg.getValue();
        
        options.pos_quantization_bits        = pos_quantization_bits       .getValue();
        options.tex_coords_quantization_bits = tex_coords_quantization_bits.getValue();
        options.normals_quantization_bits    = normals_quantization_bits   .getValue();
        options.compression_level            = compression_level           .getValue();
        
    }
    catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
    
    
    
    std::vector<std::string> aPositions;
    aPositions.push_back("aPosition");
    
    std::vector<std::string> aNormals;
    aNormals.push_back("aNormal");
    aNormals.push_back("aTangent");
    aNormals.push_back("aBitangent");
    
    std::vector<std::string> aTexCoords;
    aTexCoords.push_back("aTexCoord");
    aTexCoords.push_back("aTexCoord0");
    aTexCoords.push_back("aTexCoord1");
    
    
    
    
    

    kaitai::kstream* stream;
    awdbuf_t *bufferStruct;
    
    if( options.input.empty() ){
        //std:: string strIN;
        //std::cin >> strIN;
        //printf("input stdin %s\n", strIN.c_str()  );
        
        stream = new kaitai::kstream(&std::cin);
        bufferStruct = new awdbuf_t( stream );
    } else {
        printf("input file \n");
        std::ifstream file(options.input, std::ios::binary);
        stream = new kaitai::kstream(&file);
        bufferStruct = new awdbuf_t( stream );
    }
    
    

    
    
    
    printf("data size %lu \n", bufferStruct->data().size() );
    
    
    
    std::vector<awdbuf_t::attribute_t*> attribs = *bufferStruct->attributes();
    
    int stride = 0;
    for (int i=0; i < attribs.size(); i++) {
        awdbuf_t::attribute_t* attrib = attribs[i];
        stride += attrib->bytesize();
        
        //uint8_t numComps = attrib->flags() & 7;
        //attrib->name()
    }
    
    uint numverts = bufferStruct->data().size()/stride;
    
    
    printf("num verts %i \n", numverts );
    printf("stride    %i \n", stride );
    
    
    draco::PointCloudBuilder builder;
    // Initialize the builder for a given number of points (required).
    builder.Start(numverts);
    
    char* vdata = &bufferStruct->data()[0];
    
    
    
    
    
    
    // Setup encoder options.
    draco::EncoderOptions encoder_options = draco::CreateDefaultEncoderOptions();
    
    
    
    // Specify desired attributes.
    
    int offset = 0;
    for (int i=0; i < attribs.size(); i++) {
        awdbuf_t::attribute_t* attrib = attribs[i];
        
        uint8_t numComps = attrib->flags() & 7;
        
        //printf("num comps    %i, %i \n", numComps, attrib->bytesize() );
        
        std::string name = attrib->name()->value();
        
        draco::GeometryAttribute::Type type = draco::GeometryAttribute::GENERIC;
        int quantization_bits = 0;
        
        
        if (std::find(aPositions.begin(), aPositions.end(), name ) != aPositions.end())
        {
            type = draco::GeometryAttribute::POSITION;
            quantization_bits = options.pos_quantization_bits;
        }
        else if (std::find(aNormals.begin(), aNormals.end(), name ) != aNormals.end())
        {
            type = draco::GeometryAttribute::NORMAL;
            quantization_bits = options.normals_quantization_bits;
        }
        else if (std::find(aTexCoords.begin(), aTexCoords.end(), name ) != aTexCoords.end())
        {
            type = draco::GeometryAttribute::TEX_COORD;
            quantization_bits = options.tex_coords_quantization_bits;
        }
        
        
        int att_id =
            builder.AddAttribute(type, numComps, draco::DT_FLOAT32);
        
        builder.SetAttributeValuesForAllPoints( att_id, &vdata[offset], stride );
        
        if( quantization_bits > 0 )
            encoder_options.GetAttributeOptions( att_id )->SetInt("quantization_bits", quantization_bits);
        
        offset += attrib->bytesize();
    }
    
    
    // Get the final PointCloud.
    constexpr bool deduplicate_points = false;
    std::unique_ptr<draco::PointCloud> pc = builder.Finalize(deduplicate_points);
    
    
    
    
    // Convert compression level to speed (that 0 = slowest, 10 = fastest).
    const int speed = 10 - options.compression_level;
    draco::SetSpeedOptions(&encoder_options, speed, speed);
    
    if (options.output.empty()) {
        // Create a default output file by attaching .drc to the input file name.
        options.output = options.input + ".drc";
    }
    
    
    PrintOptions(*pc.get(), options);
    
    int ret = -1;
    ret = EncodePointCloudToFile(*pc.get(), encoder_options, options.output);
    
    if (ret != -1 && options.compression_level < 10) {
        printf(
               "For better compression, increase the compression level '-cl' (up to "
               "10).\n\n");
    }
    
    return ret;
    
    

    
    return 0;
}

